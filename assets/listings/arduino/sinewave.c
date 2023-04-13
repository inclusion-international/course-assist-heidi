// Import TensorFlow components
#include "TensorFlowLite.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h"

// Sine model
#include "sine_model.h"

#define DEBUG 0

// Settings
constexpr int led_pin = 2;
// PI
constexpr float pi = 3.14159265;
// Frequency (Hz) of sinewave
constexpr float freq = 0.5;
// Period (microseconds)
constexpr float period = (1 / freq) * 1000000;

// TFLite globals, used for compatibility with Arduino-style sketches
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* model_input = nullptr;
TfLiteTensor* model_output = nullptr;

// Create an area of memory to use for input, output, and other TensorFlow
// arrays. You'll need to adjust this by compiling, running, and looking
// for errors.
constexpr int kTensorArenaSize = 5 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace

void setup() {
#if DEBUG
  while (!Serial)
    ;
#endif

  // Let's make an LED vary in brightness
  pinMode(led_pin, OUTPUT);

  // Set up logging (will report to Serial)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure
  model = tflite::GetModel(sine_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report("Model version does not match Schema");
    while (1)
      ;
  } else {
#if DEBUG
    Serial.print("Model version: ");
    Serial.println(model->version());
    Serial.print("TensorFlow Lite Schema Version: ");
    Serial.println(TFLITE_SCHEMA_VERSION);
#endif
  }

  // Pull in only needed operations (should match NN layers)
  static tflite::MicroMutableOpResolver<1> micro_mutable_op_resolver;
  micro_mutable_op_resolver.AddFullyConnected();

  // Build an interpreter to run the model
  static tflite::MicroInterpreter static_interpreter(
      model, micro_mutable_op_resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    error_reporter->Report("AllocateTensors() failed");
    while (1)
      ;
  }

  // Assign model input and output buffers (tensors) to pointers
  model_input = interpreter->input(0);
  model_output = interpreter->output(0);

  // Get information about the memory area to use for the model's input
  // Supported data types:
  // https://github.com/tensorflow/tflite-micro/blob/main/tensorflow/lite/c/common.h
  // https://github.com/tensorflow/tflite-micro/blob/main/tensorflow/lite/core/c/c_api_types.h
#if DEBUG
  Serial.print("Number of dimensions: ");
  Serial.println(model_input->dims->size);
  Serial.print("Dim 1 size: ");
  Serial.println(model_input->dims->data[0]);
  Serial.print("Dim 2 size: ");
  Serial.println(model_input->dims->data[1]);
  Serial.print("Input type: ");
  Serial.println(model_input->type);
#endif
}

void loop() {
#if DEBUG
  unsigned long start_timestamp = micros();
#endif

  // Get current timestamp and modulo with period
  unsigned long timestamp = micros();
  timestamp = timestamp % (unsigned long)period;

  // Calculate x value to feed to the model
  float x_val = ((float)timestamp * 2 * pi) / period;

  // Copy value to input buffer (tensor)
  model_input->data.f[0] = x_val;

  // Run inference
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    error_reporter->Report("Invoke failed on input: %f\n", x_val);
  }

  // Read predicted y value from output buffer (tensor)
  float y_val = model_output->data.f[0];

  // Translate to a PWM LED brightness
  int brightness = (int)(255 * y_val);
  analogWrite(led_pin, brightness);

  // Print value
  Serial.println(y_val);

#if DEBUG
  Serial.print("Time for inference (us): ");
  Serial.println(micros() - start_timestamp);
#endif
}