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
source=("git+https://github.com/HurleybirdJr/hamburger-hurley.git#hurley_arch_runner")
sha256sums=("SKIP")

#pkgver() {
#  cd "$pkgname"
#  printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short=7 HEAD)"
#}

build() {
	cd "$pkgname"
	cmake -B "build_linux" -G "Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING=Release
	cmake --build "build_linux" --config Release --target Hamburger_VST3 -j
  cmake --build "build_linux" --config Release --target Hamburger_CLAP -j
}

package() {
	cd "$pkgname"
	install -Dm755 ./Hamburger "$pkgdir/usr/bin/hamburger"
	install -Dm644 ./README.md "$pkgdir/usr/share/doc/$pkgname"
}