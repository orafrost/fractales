NAME= fractale-generator

C_FILE = invert1r.c

O_FILE = $(C_FILE:.c=.o)

TPO_FILE = $(C_FILE:.c=.Tpo)

SO_FILE = $(C_FILE:.c=.so)

XML_FILE = frei0r_fractale.xml

FREI0R_PATH = /usr/local/lib/frei0r-1/

KDENLIVE_PATH = /usr/share/kdenlive/effects/


all: $(NAME)

$(NAME):
	gcc -DHAVE_CONFIG_H -I./include -g -O2 -MT invert1r.lo -MD -MP -MF .deps/$(TPO_FILE) -c $(C_FILE)  -fPIC -DPIC -o $(O_FILE)
	gcc -shared  $(O_FILE) -lm -Wl,-soname -Wl,$(SO_FILE) -o $(SO_FILE)
	test -d $(FREI0R_PATH) || sudo mkdir -p $(FREI0R_PATH) && sudo cp $(SO_FILE) $(FREI0R_PATH)

kde:
	sudo cp $(XML_FILE) $(KDENLIVE_PATH)

clean:
	rm -rf $(O_FILE)

fclean: clean
	rm -rf $(SO_FILE)
