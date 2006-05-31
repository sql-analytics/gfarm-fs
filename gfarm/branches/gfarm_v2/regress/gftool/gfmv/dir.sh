#!/bin/sh

. ./regress.conf

trap 'gfrmdir $gftmp/yyy $gftmp/xxx $gftmp; exit $exit_trap' $trap_sigs
trap 'gfrmdir $gftmp/yyy $gftmp; exit $exit_code' 0

if gfmkdir $gftmp &&
   gfmkdir $gftmp/xxx &&
   gfmv $gftmp/xxx $gftmp/yyy &&
   [ x"`gfls -d $gftmp/xxx`" = x"" ] &&
   [ x"`gfls -d $gftmp/yyy`" = x"$gftmp/yyy" ]; then
	exit_code=$exit_pass
fi
