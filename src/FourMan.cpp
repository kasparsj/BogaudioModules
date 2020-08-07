
#include "FourMan.hpp"

void FourMan::reset() {
	for (int i = 0; i < 4; i++) {
		_trigger[i].reset();
		_pulse[i].process(10.0f);
	}
}

void FourMan::sampleRateChange() {
	_sampleTime = APP->engine->getSampleTime();
}

void FourMan::processAll(const ProcessArgs& args) {
	bool initialPulse = false;
	if (_initialDelay && !_initialDelay->next()) {
		initialPulse = true;
		delete _initialDelay;
		_initialDelay = NULL;
	}

	for (int i = 0; i < 4; i++) {
		bool high = _trigger[i].process(params[TRIGGER1_PARAM + i].getValue()) || _trigger[i].isHigh() || (initialPulse && _triggerOnLoad && _shouldTriggerOnLoad);
		if (high) {
			_pulse[i].trigger(0.001f);
		}
		high = _pulse[i].process(_sampleTime);

		outputs[OUT1_OUTPUT + i].setVoltage(high ? 5.0f : 0.0f);
	}
}

struct FourManWidget : TriggerOnLoadModuleWidget {
	static constexpr int hp = 3;

	FourManWidget(FourMan* module)
	: TriggerOnLoadModuleWidget("Trigger on load")
	{
		setModule(module);
		box.size = Vec(RACK_GRID_WIDTH * hp, RACK_GRID_HEIGHT);
		setPanel(box.size, "FourMan");
		createScrews();

		// generated by svg_widgets.rb
		auto trigger1ParamPosition = Vec(13.5, 22.0);
		auto trigger2ParamPosition = Vec(13.5, 102.0);
		auto trigger3ParamPosition = Vec(13.5, 182.0);
		auto trigger4ParamPosition = Vec(13.5, 262.0);

		auto out1OutputPosition = Vec(10.5, 57.0);
		auto out2OutputPosition = Vec(10.5, 137.0);
		auto out3OutputPosition = Vec(10.5, 217.0);
		auto out4OutputPosition = Vec(10.5, 297.0);
		// end generated by svg_widgets.rb

		addParam(createParam<Button18>(trigger1ParamPosition, module, FourMan::TRIGGER1_PARAM));
		addParam(createParam<Button18>(trigger2ParamPosition, module, FourMan::TRIGGER2_PARAM));
		addParam(createParam<Button18>(trigger3ParamPosition, module, FourMan::TRIGGER3_PARAM));
		addParam(createParam<Button18>(trigger4ParamPosition, module, FourMan::TRIGGER4_PARAM));

		addOutput(createOutput<Port24>(out1OutputPosition, module, FourMan::OUT1_OUTPUT));
		addOutput(createOutput<Port24>(out2OutputPosition, module, FourMan::OUT2_OUTPUT));
		addOutput(createOutput<Port24>(out3OutputPosition, module, FourMan::OUT3_OUTPUT));
		addOutput(createOutput<Port24>(out4OutputPosition, module, FourMan::OUT4_OUTPUT));
	}
};

Model* modelFourMan = createModel<FourMan, FourManWidget>("Bogaudio-FourMan", "4MAN", "Quad button-controlled gates / triggers", "Controller", "Quad");
