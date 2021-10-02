
#include "PolyOff16.hpp"

#define OFFSET_FIRST "offset_first"

json_t* PolyOff16::saveToJson(json_t* root) {
	root = OutputRangeModule<BGModule>::saveToJson(root);
	json_object_set_new(root, OFFSET_FIRST, json_boolean(_offsetFirst));
	return root;
}

void PolyOff16::loadFromJson(json_t* root) {
	OutputRangeModule<BGModule>::loadFromJson(root);
	json_t* of = json_object_get(root, OFFSET_FIRST);
	if (of) {
		_offsetFirst = json_boolean_value(of);
	}
}

void PolyOff16::processAll(const ProcessArgs& args) {
	int cn = 1;
	if (inputs[IN_INPUT].isConnected()) {
		cn = clamp(inputs[IN_INPUT].getChannels(), 1, 16);
		outputs[OUT_OUTPUT].setChannels(cn);

		for (int c = 0; c < cn; ++c) {
			float offset = clamp(params[OFFSET1_PARAM + 2 * c].getValue(), -1.0f, 1.0f);
			if (inputs[CV1_INPUT + c].isConnected()) {
				offset *= clamp(inputs[CV1_INPUT + c].getVoltage() / 5.0f, -1.0f, 1.0f);
			}
			offset += _rangeOffset;
			offset *= _rangeScale;

			float scale = clamp(params[SCALE1_PARAM + 2 * c].getValue(), -1.0f, 1.0f);

			float out = inputs[IN_INPUT].getPolyVoltage(c);
			if (_offsetFirst) {
				out += offset;
				out *= scale;
			}
			else {
				out *= scale;
				out += offset;
			}
			outputs[OUT_OUTPUT].setVoltage(clamp(out, -12.0f, 12.0f), c);
		}
	}
	else {
		cn = clamp(params[CHANNELS_PARAM].getValue(), 1.0f, 16.0f);
		outputs[OUT_OUTPUT].setChannels(cn);

		for (int c = 0; c < cn; ++c) {
			float offset = clamp(params[OFFSET1_PARAM + 2 * c].getValue(), -1.0f, 1.0f);
			offset += _rangeOffset;
			offset *= _rangeScale;

			float scale = clamp(params[SCALE1_PARAM + 2 * c].getValue(), -1.0f, 1.0f);

			float out = inputs[CV1_INPUT + c].getVoltage();
			if (_offsetFirst) {
				out += offset;
				out *= scale;
			}
			else {
				out *= scale;
				out += offset;
			}
			outputs[OUT_OUTPUT].setVoltage(clamp(out, -12.0f, 12.0f), c);
		}
	}

	for (int c = 0; c < 16; ++c) {
		lights[CHANNEL1_LIGHT + c].value = (c < cn) * 1.0f;
	}
}

struct PolyOff16Widget : BGModuleWidget {
	static constexpr int hp = 16;

	PolyOff16Widget(PolyOff16* module) {
		setModule(module);
		box.size = Vec(RACK_GRID_WIDTH * hp, RACK_GRID_HEIGHT);
		setPanel(box.size, "PolyOff16");
		createScrews();

		// generated by svg_widgets.rb
		auto offset1ParamPosition = Vec(62.5, 33.0);
		auto scale1ParamPosition = Vec(93.5, 33.0);
		auto offset2ParamPosition = Vec(62.5, 68.0);
		auto scale2ParamPosition = Vec(93.5, 68.0);
		auto offset3ParamPosition = Vec(62.5, 103.0);
		auto scale3ParamPosition = Vec(93.5, 103.0);
		auto offset4ParamPosition = Vec(62.5, 138.0);
		auto scale4ParamPosition = Vec(93.5, 138.0);
		auto offset5ParamPosition = Vec(62.5, 173.0);
		auto scale5ParamPosition = Vec(93.5, 173.0);
		auto offset6ParamPosition = Vec(62.5, 208.0);
		auto scale6ParamPosition = Vec(93.5, 208.0);
		auto offset7ParamPosition = Vec(62.5, 243.0);
		auto scale7ParamPosition = Vec(93.5, 243.0);
		auto offset8ParamPosition = Vec(62.5, 278.0);
		auto scale8ParamPosition = Vec(93.5, 278.0);
		auto offset9ParamPosition = Vec(182.5, 33.0);
		auto scale9ParamPosition = Vec(213.5, 33.0);
		auto offset10ParamPosition = Vec(182.5, 68.0);
		auto scale10ParamPosition = Vec(213.5, 68.0);
		auto offset11ParamPosition = Vec(182.5, 103.0);
		auto scale11ParamPosition = Vec(213.5, 103.0);
		auto offset12ParamPosition = Vec(182.5, 138.0);
		auto scale12ParamPosition = Vec(213.5, 138.0);
		auto offset13ParamPosition = Vec(182.5, 173.0);
		auto scale13ParamPosition = Vec(213.5, 173.0);
		auto offset14ParamPosition = Vec(182.5, 208.0);
		auto scale14ParamPosition = Vec(213.5, 208.0);
		auto offset15ParamPosition = Vec(182.5, 243.0);
		auto scale15ParamPosition = Vec(213.5, 243.0);
		auto offset16ParamPosition = Vec(182.5, 278.0);
		auto scale16ParamPosition = Vec(213.5, 278.0);
		auto channelsParamPosition = Vec(83.0, 332.0);

		auto cv1InputPosition = Vec(10.5, 29.0);
		auto cv2InputPosition = Vec(10.5, 64.0);
		auto cv3InputPosition = Vec(10.5, 99.0);
		auto cv4InputPosition = Vec(10.5, 134.0);
		auto cv5InputPosition = Vec(10.5, 169.0);
		auto cv6InputPosition = Vec(10.5, 204.0);
		auto cv7InputPosition = Vec(10.5, 239.0);
		auto cv8InputPosition = Vec(10.5, 274.0);
		auto cv9InputPosition = Vec(130.5, 29.0);
		auto cv10InputPosition = Vec(130.5, 64.0);
		auto cv11InputPosition = Vec(130.5, 99.0);
		auto cv12InputPosition = Vec(130.5, 134.0);
		auto cv13InputPosition = Vec(130.5, 169.0);
		auto cv14InputPosition = Vec(130.5, 204.0);
		auto cv15InputPosition = Vec(130.5, 239.0);
		auto cv16InputPosition = Vec(130.5, 274.0);
		auto inInputPosition = Vec(118.5, 322.0);

		auto outOutputPosition = Vec(149.5, 322.0);

		auto channel1LightPosition = Vec(46.8, 43.0);
		auto channel2LightPosition = Vec(46.8, 78.0);
		auto channel3LightPosition = Vec(46.8, 113.0);
		auto channel4LightPosition = Vec(46.8, 148.0);
		auto channel5LightPosition = Vec(46.8, 183.0);
		auto channel6LightPosition = Vec(46.8, 218.0);
		auto channel7LightPosition = Vec(46.8, 253.0);
		auto channel8LightPosition = Vec(46.8, 288.0);
		auto channel9LightPosition = Vec(166.8, 43.0);
		auto channel10LightPosition = Vec(166.8, 78.0);
		auto channel11LightPosition = Vec(166.8, 113.0);
		auto channel12LightPosition = Vec(166.8, 148.0);
		auto channel13LightPosition = Vec(166.8, 183.0);
		auto channel14LightPosition = Vec(166.8, 218.0);
		auto channel15LightPosition = Vec(166.8, 253.0);
		auto channel16LightPosition = Vec(166.8, 288.0);
		// end generated by svg_widgets.rb

		addParam(createParam<Knob16>(offset1ParamPosition, module, PolyOff16::OFFSET1_PARAM));
		addParam(createParam<Knob16>(scale1ParamPosition, module, PolyOff16::SCALE1_PARAM));
		addParam(createParam<Knob16>(offset2ParamPosition, module, PolyOff16::OFFSET2_PARAM));
		addParam(createParam<Knob16>(scale2ParamPosition, module, PolyOff16::SCALE2_PARAM));
		addParam(createParam<Knob16>(offset3ParamPosition, module, PolyOff16::OFFSET3_PARAM));
		addParam(createParam<Knob16>(scale3ParamPosition, module, PolyOff16::SCALE3_PARAM));
		addParam(createParam<Knob16>(offset4ParamPosition, module, PolyOff16::OFFSET4_PARAM));
		addParam(createParam<Knob16>(scale4ParamPosition, module, PolyOff16::SCALE4_PARAM));
		addParam(createParam<Knob16>(offset5ParamPosition, module, PolyOff16::OFFSET5_PARAM));
		addParam(createParam<Knob16>(scale5ParamPosition, module, PolyOff16::SCALE5_PARAM));
		addParam(createParam<Knob16>(offset6ParamPosition, module, PolyOff16::OFFSET6_PARAM));
		addParam(createParam<Knob16>(scale6ParamPosition, module, PolyOff16::SCALE6_PARAM));
		addParam(createParam<Knob16>(offset7ParamPosition, module, PolyOff16::OFFSET7_PARAM));
		addParam(createParam<Knob16>(scale7ParamPosition, module, PolyOff16::SCALE7_PARAM));
		addParam(createParam<Knob16>(offset8ParamPosition, module, PolyOff16::OFFSET8_PARAM));
		addParam(createParam<Knob16>(scale8ParamPosition, module, PolyOff16::SCALE8_PARAM));
		addParam(createParam<Knob16>(offset9ParamPosition, module, PolyOff16::OFFSET9_PARAM));
		addParam(createParam<Knob16>(scale9ParamPosition, module, PolyOff16::SCALE9_PARAM));
		addParam(createParam<Knob16>(offset10ParamPosition, module, PolyOff16::OFFSET10_PARAM));
		addParam(createParam<Knob16>(scale10ParamPosition, module, PolyOff16::SCALE10_PARAM));
		addParam(createParam<Knob16>(offset11ParamPosition, module, PolyOff16::OFFSET11_PARAM));
		addParam(createParam<Knob16>(scale11ParamPosition, module, PolyOff16::SCALE11_PARAM));
		addParam(createParam<Knob16>(offset12ParamPosition, module, PolyOff16::OFFSET12_PARAM));
		addParam(createParam<Knob16>(scale12ParamPosition, module, PolyOff16::SCALE12_PARAM));
		addParam(createParam<Knob16>(offset13ParamPosition, module, PolyOff16::OFFSET13_PARAM));
		addParam(createParam<Knob16>(scale13ParamPosition, module, PolyOff16::SCALE13_PARAM));
		addParam(createParam<Knob16>(offset14ParamPosition, module, PolyOff16::OFFSET14_PARAM));
		addParam(createParam<Knob16>(scale14ParamPosition, module, PolyOff16::SCALE14_PARAM));
		addParam(createParam<Knob16>(offset15ParamPosition, module, PolyOff16::OFFSET15_PARAM));
		addParam(createParam<Knob16>(scale15ParamPosition, module, PolyOff16::SCALE15_PARAM));
		addParam(createParam<Knob16>(offset16ParamPosition, module, PolyOff16::OFFSET16_PARAM));
		addParam(createParam<Knob16>(scale16ParamPosition, module, PolyOff16::SCALE16_PARAM));
		{
			auto w = createParam<Knob16>(channelsParamPosition, module, PolyOff16::CHANNELS_PARAM);
			dynamic_cast<Knob*>(w)->snap = true;
			addParam(w);
		}

		addInput(createInput<Port24>(cv1InputPosition, module, PolyOff16::CV1_INPUT));
		addInput(createInput<Port24>(cv2InputPosition, module, PolyOff16::CV2_INPUT));
		addInput(createInput<Port24>(cv3InputPosition, module, PolyOff16::CV3_INPUT));
		addInput(createInput<Port24>(cv4InputPosition, module, PolyOff16::CV4_INPUT));
		addInput(createInput<Port24>(cv5InputPosition, module, PolyOff16::CV5_INPUT));
		addInput(createInput<Port24>(cv6InputPosition, module, PolyOff16::CV6_INPUT));
		addInput(createInput<Port24>(cv7InputPosition, module, PolyOff16::CV7_INPUT));
		addInput(createInput<Port24>(cv8InputPosition, module, PolyOff16::CV8_INPUT));
		addInput(createInput<Port24>(cv9InputPosition, module, PolyOff16::CV9_INPUT));
		addInput(createInput<Port24>(cv10InputPosition, module, PolyOff16::CV10_INPUT));
		addInput(createInput<Port24>(cv11InputPosition, module, PolyOff16::CV11_INPUT));
		addInput(createInput<Port24>(cv12InputPosition, module, PolyOff16::CV12_INPUT));
		addInput(createInput<Port24>(cv13InputPosition, module, PolyOff16::CV13_INPUT));
		addInput(createInput<Port24>(cv14InputPosition, module, PolyOff16::CV14_INPUT));
		addInput(createInput<Port24>(cv15InputPosition, module, PolyOff16::CV15_INPUT));
		addInput(createInput<Port24>(cv16InputPosition, module, PolyOff16::CV16_INPUT));
		addInput(createInput<Port24>(inInputPosition, module, PolyOff16::IN_INPUT));

		addOutput(createOutput<Port24>(outOutputPosition, module, PolyOff16::OUT_OUTPUT));

		addChild(createLight<BGTinyLight<GreenLight>>(channel1LightPosition, module, PolyOff16::CHANNEL1_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel2LightPosition, module, PolyOff16::CHANNEL2_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel3LightPosition, module, PolyOff16::CHANNEL3_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel4LightPosition, module, PolyOff16::CHANNEL4_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel5LightPosition, module, PolyOff16::CHANNEL5_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel6LightPosition, module, PolyOff16::CHANNEL6_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel7LightPosition, module, PolyOff16::CHANNEL7_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel8LightPosition, module, PolyOff16::CHANNEL8_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel9LightPosition, module, PolyOff16::CHANNEL9_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel10LightPosition, module, PolyOff16::CHANNEL10_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel11LightPosition, module, PolyOff16::CHANNEL11_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel12LightPosition, module, PolyOff16::CHANNEL12_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel13LightPosition, module, PolyOff16::CHANNEL13_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel14LightPosition, module, PolyOff16::CHANNEL14_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel15LightPosition, module, PolyOff16::CHANNEL15_LIGHT));
		addChild(createLight<BGTinyLight<GreenLight>>(channel16LightPosition, module, PolyOff16::CHANNEL16_LIGHT));
	}

	void contextMenu(Menu* menu) override {
		OutputRangeOptionMenuItem::addOutputRangeOptionsToMenu(module, menu);

		auto m = dynamic_cast<PolyOff16*>(module);
		assert(m);

		OptionsMenuItem* ooo = new OptionsMenuItem("Order of operations");
		ooo->addItem(OptionMenuItem("Scale, then offset", [m]() { return !m->_offsetFirst; }, [m]() { m->_offsetFirst = false; }));
		ooo->addItem(OptionMenuItem("Offset, then scale", [m]() { return m->_offsetFirst; }, [m]() { m->_offsetFirst = true; }));
		OptionsMenuItem::addToMenu(ooo, menu);
	}
};

Model* modelPolyOff16 = createModel<PolyOff16, PolyOff16Widget>("Bogaudio-PolyOff16", "POLYOFF16", "Polyphonic per-channel offset and scale", "Polyphonic");
