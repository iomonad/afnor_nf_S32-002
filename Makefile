TARGET = afnor_nf_s32_002

all:
	cc -pedantic -lhackrf -lusb-1.0 -O2 afnor_nf_s32_002.c -o ${TARGET}

clean:
	rm -f ${TARGET}
