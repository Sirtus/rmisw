compute:
	gcc -o lab1 lab1.c
	./lab1

#example: make gen_matrix N=5
gen_matrix:
ifndef N
	echo "Specify N. Example: make gen_matrix N=5"
else
	python3 generate_matrix.py $(N)
endif

all:
	$(MAKE) gen_matrix
	$(MAKE) compute
