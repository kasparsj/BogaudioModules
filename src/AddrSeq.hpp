#pragma once

#include "bogaudio.hpp"
#include "dsp/signal.hpp"

using namespace bogaudio::dsp;

extern Model* modelAddrSeq;

namespace bogaudio {

struct AddrSeq : Module {
	enum ParamsIds {
		STEPS_PARAM,
		DIRECTION_PARAM,
		SELECT_PARAM,
		OUT1_PARAM,
		OUT2_PARAM,
		OUT3_PARAM,
		OUT4_PARAM,
		OUT5_PARAM,
		OUT6_PARAM,
		OUT7_PARAM,
		OUT8_PARAM,
		NUM_PARAMS
	};

	enum InputsIds {
		CLOCK_INPUT,
		RESET_INPUT,
		SELECT_INPUT,
		NUM_INPUTS
	};

	enum OutputsIds {
		OUT_OUTPUT,
		NUM_OUTPUTS
	};

	enum LightsIds {
		OUT1_LIGHT,
		OUT2_LIGHT,
		OUT3_LIGHT,
		OUT4_LIGHT,
		OUT5_LIGHT,
		OUT6_LIGHT,
		OUT7_LIGHT,
		OUT8_LIGHT,
		NUM_LIGHTS
	};

	Trigger _clock;
	Trigger _reset;
	bogaudio::dsp::Timer _timer;
	int _step;
	bool _selectOnClock = false;
	int _select = 0;

	AddrSeq() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(STEPS_PARAM, 1.0f, 8.0f, 8.0f, "steps");
		configParam(DIRECTION_PARAM, 0.0f, 1.0f, 1.0f, "direction");
		configParam(SELECT_PARAM, 0.0f, 7.0f, 0.0f, "select");
		configParam(OUT1_PARAM, -1.0f, 1.0f, 0.0f, "out1");
		configParam(OUT2_PARAM, -1.0f, 1.0f, 0.0f, "out2");
		configParam(OUT3_PARAM, -1.0f, 1.0f, 0.0f, "out3");
		configParam(OUT4_PARAM, -1.0f, 1.0f, 0.0f, "out4");
		configParam(OUT5_PARAM, -1.0f, 1.0f, 0.0f, "out5");
		configParam(OUT6_PARAM, -1.0f, 1.0f, 0.0f, "out6");
		configParam(OUT7_PARAM, -1.0f, 1.0f, 0.0f, "out7");
		configParam(OUT8_PARAM, -1.0f, 1.0f, 0.0f, "out8");

		onReset();
		onSampleRateChange();
	}

	void onReset() override;
	void onSampleRateChange() override;
	json_t* dataToJson() override;
	void dataFromJson(json_t* root) override;
	void process(const ProcessArgs& args) override;
};

} // namespace bogaudio
