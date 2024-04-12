# Maintainer: Will Hurley <will@hurleybirdjr.com>
pkgname="hamburger"
pkgver="r202.fef77ab"
pkgrel=1
pkgdesc="A distortion and dynamics plugin designed for sonic destruction and tasteful saturation."
arch=("x86_64")
url="https://aviaryaudio.com/plugins/hamburgerv2"
license=('GPLv3')
depends=()
makedepends=("git", "gcc")
options=()
# install=
source=("$pkgname-$pkgver.tar.gz"
        "$pkgname-$pkgver.patch")
source=("Hamburger::git://github.com/Davit-G/Hamburger.git")
sha256sums=("SKIP")

pkgver() {
	cd "$pkgname"
    printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

prepare() {
	cd "$pkgname-$pkgver"
	patch -p1 -i "$srcdir/$pkgname-$pkgver.patch"
}

build() {
	cd "$pkgname-$pkgver"
	./configure --prefix=/usr
	make
}

check() {
	cd "$pkgname-$pkgver"
	make -k check
}

package() {
	cd "$pkgname-$pkgver"
	make DESTDIR="$pkgdir/" install
}
