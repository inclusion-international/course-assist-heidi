uint8_t i, TXBuf[6];

ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
UBRR0H = (unsigned char)(ubrr >> 8);
UBRR0L = (unsigned char)(ubrr & 0xff);
UCSR0A |= (1 << U2X0);
UCSR0B = (1 << TXEN0) | (1 << RXEN0);
UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

ADCSRA |= (1 << ADSC);

while (ADCSRA & (1 << ADSC))
  ;

itoa(ADC, TXBuf, 10);

while (TXBuf[i]) {
  while (!(UCSR0A & (1 << URDE0)))
    ;
  UDR0 = TXBuf[i++];
}