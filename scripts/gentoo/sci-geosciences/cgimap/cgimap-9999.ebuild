# Copyright 1999-2013 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2

EAPI="5"

inherit git-2

DESCRIPTION="Mirror of CGImap, the optimized implementation of the OpenStreetMap \"/map\" API call."
HOMEPAGE="https://github.com/openstreetmap/cgimap"
EGIT_REPO_URI="https://github.com/openstreetmap/cgimap.git"

LICENSE="GPL-2"
SLOT="0/9999"
KEYWORDS=""

# Doc and man will be installed by default (use FEATURES="nodoc noman" to disable it).
IUSE="+apidb +pgsnapshot api07 experimental yajl static test"

RDEPEND="
    dev-libs/libxml2
    dev-libs/libpqxx
    dev-libs/fcgi
    dev-libs/boost:=[threads]
    dev-libs/libmemcached
    dev-libs/crypto++
    yajl? ( dev-libs/yajl )
    test? (
        dev-db/postgresql
        dev-db/postgis
    )
"
DEPEND="${RDEPEND}"

src_unpack() {
    git-2_src_unpack
}

src_prepare() {
    ./autogen.sh || die "autogen script failed"
}

src_configure() {
    econf \
        $(use_enable apidb) \
        $(use_enable pgsnapshot) \
        $(use_enable api07) \
        $(use_enable experimental) \
        $(use_enable yajl) \
        $(use_enable static)
}

src_test() {
    make check || die "make check failed"
}
