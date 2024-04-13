# Maintainer: Will Hurley <will@hurleybirdjr.com>
pkgname="hamburger-git"
pkgver="r203.a4bd424"
pkgrel=1
pkgdesc="A distortion and dynamics plugin designed for sonic destruction and tasteful saturation."
arch=("x86_64")
url="https://aviaryaudio.com/plugins/hamburgerv2"
license=('GPL-3.0-only')
depends=(gcc-libs glibc freetype2 alsa-lib)
makedepends=(base-devel git cmake libx11 libxrandr libxcursor libxinerama alsa-lib freetype2)
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
	cmake --build "build_linux" --config Release --target Hamburger_VST3 --parallel $(($(nproc) - 1))
	cmake --build "build_linux" --config Release --target Hamburger_CLAP --parallel $(($(nproc) - 1))
    cmake --build "build_linux" --config Release --target Hamburger_Standalone --parallel $(($(nproc) - 1))
}

package() {
	depends=(gcc-libs glibc freetype2 alsa-lib)

	mkdir -p "${pkgdir}/usr/lib/vst3/"
    find . -name '*.vst3' -type d -exec cp -ar {} "${pkgdir}/usr/lib/vst3/" \;

    mkdir -p "${pkgdir}/usr/lib/clap/"
    find . -name '*.clap' -exec cp {} "${pkgdir}/usr/lib/clap/" \;

	install -Dm755 -T "$srcdir/hamburger-hurley/build_linux/Hamburger_artefacts/Release/Standalone/Hamburger" "${pkgdir}/usr/bin/Hamburger"
}
