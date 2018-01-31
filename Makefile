MPY_FILES=bme280 pms7003 sensor_manager webapp
PY_FILES=boot main
PROJECT_DIR=smogomierz
PORT=/dev/ttyUSB0

DEPLOYED_MPY_FILES=$(foreach file,$(MPY_FILES),deployed/$(file).mpy)
DEPLOYED_PY_FILES=$(foreach file,$(PY_FILES),deployed/$(file).py)

all: deploy

mpy/%.mpy: $(PROJECT_DIR)/%.py
	mpy-cross $< -o $@

deployed/%.mpy: mpy/%.mpy
	ampy -p $(PORT) put $<
	@cp $< $@

deployed/%.py: $(PROJECT_DIR)/%.py
	ampy -p $(PORT) put $<
	@cp $< $@

deploy: $(DEPLOYED_MPY_FILES) $(DEPLOYED_PY_FILES)

clean:
	rm -rf mpy/*
	rm -rf deployed/*
