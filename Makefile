.POSIX:

SRCDIR = src
OUTDIR = bin
TESTDIR= test
TREES = datastructures/trees
REDBLACK = ${TREES}/redblack
CXXFLAGS = -std=c++17 -I${SRCDIR}/${TREES}/
CXX = g++

.SILENT: clean

tests:
	@echo "Creating ouput directory [${OUTDIR}/${TREES}]"
	mkdir -p ${OUTDIR}/${TREES}

	@echo "Compiling and linking..."
	${CXX} ${CXXFLAGS} -c ${SRCDIR}/${REDBLACK}.cpp -o ${OUTDIR}/${REDBLACK}.o
	${CXX} ${CXXFLAGS} -c -x c++ ${TESTDIR}/${REDBLACK}.tpp -o ${OUTDIR}/${REDBLACK}.t
	${CXX} ${CXXFLAGS} \
		${OUTDIR}/${REDBLACK}.o \
		${OUTDIR}/${REDBLACK}.t \
		-o ${OUTDIR}/${REDBLACK}

	@echo "Running test..."
	@${OUTDIR}/${REDBLACK}

clean:
	rm -rf bin/