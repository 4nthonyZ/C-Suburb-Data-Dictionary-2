# 定义编译器和编译标志
CC = gcc
CFLAGS = -Wall -g

# 定义目标文件
TARGETS = dict3 dict4

# 定义源文件
SRCS_DICT3 = dict3.c Linked_list.c
SRCS_DICT4 = dict4.c PatriciaTree.c

# 生成可执行文件
dict3: $(SRCS_DICT3)
	$(CC) $(CFLAGS) -o dict3 $(SRCS_DICT3)

# 空的 dict4，目前只是占位符
dict4: $(SRCS_DICT4)
	$(CC) $(CFLAGS) -o dict4 $(SRCS_DICT4)

# 运行测试（针对 dict3 的测试）
test1:
	./dict3 3 tests/dataset_1.csv output.out < tests/test1.in

test15:
	./dict3 3 tests/dataset_15.csv output.out < tests/test15.in

test100:
	./dict3 3 tests/dataset_100.csv output.out < tests/test100.in

test1000:
	./dict3 3 tests/dataset_1000.csv output.out < tests/test1000.in

testfull:
	./dict3 3 tests/dataset_full.csv output.out < tests/testfull.in

# dict4 的占位符测试，可以稍后实现
test4_1:
	./dict4 4 tests/dataset_1.csv output.out < tests/test1.in

test4_15:
	./dict4 4 tests/dataset_15.csv output.out < tests/test15.in

test4_100:
	./dict4 4 tests/dataset_100.csv output.out < tests/test100.in

test4_1000:
	./dict4 4 tests/dataset_1000.csv output.out < tests/test1000.in

test4_full:
	./dict4 4 tests/dataset_full.csv output.out < tests/testfull.in

# 清理生成的文件
clean:
	rm -f $(TARGETS) output.out
