
#include "Inv.hpp"

#define SAVE_LATCHED_TO_PATCH "save_latched_to_patch"
#define LATCHED_STATE1 "latched_state1"
#define LATCHED_STATE2 "latched_state2"

void Inv::reset() {
	for (int c = 0; c < maxChannels; ++c) {
		_trigger[0][c].reset();
		_trigger[1][c].reset();
	}
}

json_t* Inv::saveToJson(json_t* root) {
	json_object_set_new(root, SAVE_LATCHED_TO_PATCH, json_boolean(_saveLatchedToPatch));
	if (_saveLatchedToPatch) {
		if (_latch[0]) {
			json_t* a = json_array();
			for (int c = 0; c < maxChannels; ++c) {
				json_array_append_new(a, json_boolean(_latchedHigh[0][c]));
			}
			json_object_set_new(root, LATCHED_STATE1, a);
		}

		if (_latch[1]) {
			json_t* a = json_array();
			for (int c = 0; c < maxChannels; ++c) {
				json_array_append_new(a, json_boolean(_latchedHigh[1][c]));
			}
			json_object_set_new(root, LATCHED_STATE2, a);
		}
	}
	return root;
}

void Inv::loadFromJson(json_t* root) {
	json_t* sl = json_object_get(root, SAVE_LATCHED_TO_PATCH);
	if (sl) {
		_saveLatchedToPatch = json_is_true(sl);
		if (_saveLatchedToPatch) {
			json_t* a1 = json_object_get(root, LATCHED_STATE1);
			if (a1 && json_array_size(a1) == maxChannels) {
				for (int c = 0; c < maxChannels; ++c) {
					json_t* ls = json_array_get(a1, c);
					if (ls && json_is_true(ls)) {
						_latchedHigh[0][c] = true;
					}
				}
			}

			json_t* a2 = json_object_get(root, LATCHED_STATE2);
			if (a2 && json_array_size(a2) == maxChannels) {
				for (int c = 0; c < maxChannels; ++c) {
					json_t* ls = json_array_get(a2, c);
					if (ls && json_is_true(ls)) {
						_latchedHigh[1][c] = true;
					}
				}
			}
		}
	}
}

void Inv::modulate() {
	_latch[0] = params[LATCH1_PARAM].getValue() > 0.5f;
	_latch[1] = params[LATCH2_PARAM].getValue() > 0.5f;
}

void Inv::processAll(const ProcessArgs& args) {
	processDual(0);
	processDual(1);
}

void Inv::processDual(int i) {
	int channels = inputs[IN1_INPUT + 2 * i].getChannels();
	outputs[OUT1_OUTPUT + i].setChannels(channels);
	int lightsOn = 0;

	for (int c = 0; c < channels; ++c) {
		bool triggered = _trigger[i][c].process(params[GATE1_PARAM + 2 * i].getValue() + inputs[GATE1_INPUT + 2 * i].getPolyVoltage(c));
		if (_latch[i]) {
			if (triggered) {
				_latchedHigh[i][c] = !_latchedHigh[i][c];
			}
		}
		else {
			_latchedHigh[i][c] = false;
		}

		float in = inputs[IN1_INPUT + 2 * i].getPolyVoltage(c);
		if (_latchedHigh[i][c] || (!_latch[i] && _trigger[i][c].isHigh())) {
			in = -in;
			++lightsOn;
		}
		outputs[OUT1_OUTPUT + i].setVoltage(in, c);
	}

	float red = lights[HIGH1_LIGHT + 2 * i].value = lightsOn / (float)channels;
	lights[LOW1_LIGHT + 2 * i].value = 1.0f - red;
}

struct InvWidget : BGModuleWidget {
	static constexpr int hp = 3;

	InvWidget(Inv* module) {
		setModule(module);
		box.size = Vec(RACK_GRID_WIDTH * hp, RACK_GRID_HEIGHT);
		setPanel(box.size, "Inv");
		createScrews();

		// generated by svg_widgets.rb
		auto gate1ParamPosition = Vec(13.5, 22.0);
		auto latch1ParamPosition = Vec(29.5, 82.0);
		auto gate2ParamPosition = Vec(13.5, 187.0);
		auto latch2ParamPosition = Vec(29.5, 247.0);

		auto gate1InputPosition = Vec(10.5, 44.0);
		auto in1InputPosition = Vec(10.5, 96.0);
		auto gate2InputPosition = Vec(10.5, 209.0);
		auto in2InputPosition = Vec(10.5, 261.0);

		auto out1OutputPosition = Vec(10.5, 134.0);
		auto out2OutputPosition = Vec(10.5, 299.0);

		auto low1LightPosition = Vec(10.5, 160.3);
		auto low2LightPosition = Vec(10.5, 325.3);
		// end generated by svg_widgets.rb

		addParam(createParam<Button18>(gate1ParamPosition, module, Inv::GATE1_PARAM));
		addParam(createParam<IndicatorButtonGreen9>(latch1ParamPosition, module, Inv::LATCH1_PARAM));
		addParam(createParam<Button18>(gate2ParamPosition, module, Inv::GATE2_PARAM));
		addParam(createParam<IndicatorButtonGreen9>(latch2ParamPosition, module, Inv::LATCH2_PARAM));

		addInput(createInput<Port24>(gate1InputPosition, module, Inv::GATE1_INPUT));
		addInput(createInput<Port24>(in1InputPosition, module, Inv::IN1_INPUT));
		addInput(createInput<Port24>(gate2InputPosition, module, Inv::GATE2_INPUT));
		addInput(createInput<Port24>(in2InputPosition, module, Inv::IN2_INPUT));

		addOutput(createOutput<Port24>(out1OutputPosition, module, Inv::OUT1_OUTPUT));
		addOutput(createOutput<Port24>(out2OutputPosition, module, Inv::OUT2_OUTPUT));

		addChild(createLight<BGSmallLight<GreenRedLight>>(low1LightPosition, module, Inv::LOW1_LIGHT));
		addChild(createLight<BGSmallLight<GreenRedLight>>(low2LightPosition, module, Inv::LOW2_LIGHT));
	}

	void contextMenu(Menu* menu) override {
		auto m = dynamic_cast<Inv*>(module);
		assert(m);
		menu->addChild(new BoolOptionMenuItem("Save latched state to patch", [m]() { return &m->_saveLatchedToPatch; }));
	}
};

Model* modelInv = createModel<Inv, InvWidget>("Bogaudio-Inv", "INV", "Dual signal inverter", "Logic", "Dual", "Polyphonic");
