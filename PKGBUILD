# Maintainer: Tomáš Henzl thenzl@seznam.cz
pkgname=cker-git
_gitname=cker
pkgver=0.0.0
pkgrel=1
pkgdesc="Command line utility for extracting text data from given url."
arch=('i686' 'x86_64')
url="http://henkye.chickenkiller.com/cker/"
license=('GPLv3')
depends=('curl')
makedepends=('git')
source=("git://github.com/henkye/cker.git")
md5sums=("SKIP")

pkgver() {
  cd $_gitname
  # Use the tag of the last commit
  git describe --always | sed 's|-|.|g'
}

build() {
  cd $_gitname
  make
}

package() {
  cd $_gitname
  make PREFIX=/usr DESTDIR="$pkgdir" install
}