FROM texlive/texlive:latest

RUN apt update

# Fonts
RUN mkdir -p /usr/share/fonts/truetype/
RUN curl https://raw.githubusercontent.com/google/fonts/main/ufl/ubuntu/Ubuntu-Light.ttf --output /usr/share/fonts/truetype/Ubuntu-Light.ttf
RUN curl https://raw.githubusercontent.com/google/fonts/main/ufl/ubuntumono/UbuntuMono-Regular.ttf --output /usr/share/fonts/truetype/UbuntuMono-Regular.ttf
# RUN curl https://raw.githubusercontent.com/google/fonts/main/ofl/notosanssc/NotoSansSC%5Bwght%5D.ttf --output /usr/share/fonts/truetype/NotoSans-SC.ttf
RUN curl https://fonts.google.com/download?family=Noto%20Sans%20SC --output /var/NotoSansSC.zip
RUN apt install -y unzip && unzip /var/NotoSansSC.zip -d /usr/share/fonts/truetype
RUN fc-cache -fsv

# Python tools
RUN apt install -y python3-setuptools
RUN mkdir -p /slides/custom
WORKDIR /slides/custom
RUN git clone https://github.com/sabicalija/FhtwStyle.git
RUN git clone https://github.com/sabicalija/VueLexer.git
RUN git clone https://github.com/xyz2tex/svg2tikz
RUN cd /slides/custom/FhtwStyle && python3 setup.py install
RUN cd /slides/custom/VueLexer && python3 setup.py install
# Inkscape
RUN curl http://ftp.at.debian.org/debian/pool/main/i/inkscape/inkscape_1.2.1+ds-1+b1_amd64.deb --output inkscape_1.2.1.deb
RUN apt install -y ./inkscape_1.2.1.deb
# https://github.com/paaguti/svg2tikz
# https://github.com/xyz2tex/svg2tikz/issues/42
# https://github.com/xyz2tex/svg2tikz/issues/62
RUN cd /slides/custom/svg2tikz && git checkout 7a9959c && python3 setup.py install

WORKDIR /slides