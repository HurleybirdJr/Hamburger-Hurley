# Maintainer: Will Hurley <will@hurleybirdjr.com>
pkgname="hamburger-hurley"
pkgver="r203.a4bd424"
pkgrel=1
pkgdesc="A distortion and dynamics plugin designed for sonic destruction and tasteful saturation."
arch=("x86_64")
url="https://aviaryaudio.com/plugins/hamburgerv2"
license=('GPLv3')
depends=()
makedepends=("base-devel" "git" "cmake" "libx11" "libxrandr" "libxcursor" "libxinerama" "alsa-lib" "freetype2")
# install=
# source=("$pkgname-$pkgver.tar.gz") - USE FOR BINARY
source=("git+https://github.com/HurleybirdJr/hamburger-hurley.git#branch=hurley_arch_runner")
sha256sums=("SKIP")

#pkgver() {
#  cd "$pkgname"
#  printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short=7 HEAD)"
#}

build() {
	cd "$pkgname"
	echo "$pkgdir"
	echo "$srcdir"
	cmake -B "build_linux" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=Release
	# cmake --build "build_linux" --config Release --target Hamburger_VST3 ---parallel $(($(nproc) - 1))
	cmake --build "build_linux" --config Release --target Hamburger_CLAP --parallel $(($(nproc) - 1))
  # cmake --build "build_linux" --config Release --target Hamburger_Standalone --parallel $(($(nproc) - 1))
}

package() {
	cd "$pkgname"
	# mkdir -p "/home/$USER/.vst3/Hamburger.vst3" || cp "$srcdir/hamburger-hurley/build_linux/Hamburger_artefacts/Release/VST3/Hamburger.vst3" "/home/$USER/.vst3/Hamburger.vst3" && chmod 755 "/home/$USER/.vst3/Hamburger.vst3"
	mkdir -p "/home/$USER/.clap" || cp "$srcdir/hamburger-hurley/build_linux/Hamburger_artefacts/Release/CLAP/Hamburger.clap" "/home/$USER/.clap/Hamburger.clap" && chmod 755 "/home/$USER/.clap/Hamburger.clap"

	# Need to figure out adding sudo to test docker
	# mv "$srcdir/hamburger-hurley/build_linux/Hamburger_artefacts/Release/Standalone/Hamburger" "/usr/bin" && chmod 644 "/usr/bin/Hamburger"
	# mkdir -p "/usr/share/licenses/$pkgname" || cp "$srcdir/hamburger-hurley/LICENSE.md" "/usr/share/licenses/$pkgname/LICENSE" && chmod 644 "/usr/share/licenses/$pkgname/LICENSE"
	# mkdir -p "/usr/share/doc/$pkgname" || cp "$srcdir/hamburger-hurley/README.md" "/usr/share/doc/$pkgname" && chmod 644 "/usr/share/doc/$pkgname"
}