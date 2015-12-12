from random import *
#s = 1
#h = 10
#w = 1

def gen_sparse_matr(s,h,w) :
    #w=1
    f = open('matrs/in_matr_' + str(s).replace('.', '') + '_' +
             str(w) + '_' + str(h) + '.txt', 'w')
    print("{:d} {:d} {:1.1f}".format(w, h, s), file=f)

    for i in range(1,w+1) :
        l = []
        for j in range(1,h+1) :
            if (random() < s) :
                l.append((uniform(1,99), j))

        for j in l :
            print("{:d} {:d} {:2.2f}".format(i, j[1], j[0]), file=f)
        print('', file=f)
    f.close()

def gen_sparse_matr_column(s,h) :
    #w=1
    f = open('matr_cols/in_matr_column_' + str(s).replace('.', '') + '_' + str(h) + '.txt', 'w')
    print("{:d} {:d} {:1.1f}".format(h, 1, s), file=f)

    # for i in range(1,w+1) :
    l = []
    for j in range(1,h+1) :
        if (random() < s) :
            l.append((uniform(1,99), j))

    for j in l :
        print("{:d} {:d} {:2.2f}".format(j[1], 1, j[0]), file=f)
    print('', file=f)
    f.close()
    
for size in [10, 100, 1000]:
	for degree in [0.1, 0.2, 0.5, 0.8, 1.0]:
		    gen_sparse_matr(degree, size, size)

for size in [10, 100, 1000]:
	for degree in [0.1, 0.2, 0.5, 0.8, 1.0]:
		    gen_sparse_matr_column(degree, size)

