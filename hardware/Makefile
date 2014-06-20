PCBNAME=closed_sesame
AUTHOR="Matthew O'Gorman <mog@rldn.net>  , Tim Heath <crashcartpro@gmail.com>"
REV=0.0.1
STACK=-
SOLDERMASK=red
SILKSCREEN=white
THICKNESS="1.2mm"
#CUSTOMPCB=/home/mog/nonapt/pcb/src/pcb
#CUSTOMPCB_ARGS=--photo-mask-colour $(SOLDERMASK) \
#	       --photo-silk-colour $(SILKSCREEN)  --photo-plating  tinned
# --photo-plating <tinned|gold|silver|copper>

all: dummy

dummy:
	@echo ""

ifeq "$(wildcard ../meatstand_common/schematics )" ""
	@echo "I didn't have meatstand_common cloned."
	@echo "I will get it for you now."
	../update_submodule.sh
	@echo "I got my dependencies I am ready to go"
	@make
endif

-include ../meatstand_common/schematics/production_Makefile
