#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

GOLDCOIND=${GOLDCOIND:-$SRCDIR/goldcoind}
GOLDCOINCLI=${GOLDCOINCLI:-$SRCDIR/goldcoin-cli}
GOLDCOINTX=${GOLDCOINTX:-$SRCDIR/goldcoin-tx}
GOLDCOINQT=${GOLDCOINQT:-$SRCDIR/qt/goldcoin-qt}

[ ! -x $GOLDCOIND ] && echo "$GOLDCOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
BTCVER=($($GOLDCOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for goldcoind if --version-string is not set,
# but has different outcomes for goldcoin-qt and goldcoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$GOLDCOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $GOLDCOIND $GOLDCOINCLI $GOLDCOINTX $GOLDCOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BTCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BTCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
