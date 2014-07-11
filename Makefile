

all:
	$(MAKE) -C server
	$(MAKE) -C libcameraclient

clean:
	$(MAKE) -C server clean
	$(MAKE) -C libcameraclient clean


