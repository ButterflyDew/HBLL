CXX = g++ # C++编译器
CXXFLAGS = -std=c++17 -Wall -O2# 编译选项

TARGET = debug # 目标可执行文件名

# 列出你的源文件
SOURCES = debug.cpp
HEADERS = global.h static_hl.h betweenness_centrality.h

# 根据需要添加更多源文件
# SOURCES += additional_file.cpp

# 生成目标的规则
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

.PHONY: clean

clean:
	rm -f $(TARGET)
