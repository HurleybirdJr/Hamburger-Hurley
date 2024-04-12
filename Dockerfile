FROM archlinux

RUN pacman -Sy --noconfirm base-devel git cmake alsa-lib libx11 libxcursor libxrandr libxinerama freetype2

RUN useradd builduser -m \
    && passwd -d builduser

RUN mkdir /home/builduser/hamburger
COPY PKGBUILD /home/builduser/hamburger

RUN chown builduser -R /home/builduser/hamburger

RUN sudo -u builduser bash -c "cd /home/builduser/hamburger && makepkg --noconfirm"
RUN pacman -Rns $(pacman -Qtdq)



WORKDIR /home/builduser

CMD ["bash"]