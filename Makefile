TARGET		:= $(notdir $(CURDIR))
SOURCES		:= source/*.c
CXXFLAGS	:= -O3
LDFLAGS		:= -lcurl

$(TARGET): $(SOURCES)
	g++ $? -o $@ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -rf $(TARGET)

.PHONY: clean
