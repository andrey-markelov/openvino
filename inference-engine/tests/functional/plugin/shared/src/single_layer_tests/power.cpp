// Copyright (C) 2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <debug.h>
#include "functional_test_utils/precision_utils.hpp"
#include "functional_test_utils/skip_tests_config.hpp"
#include "single_layer_tests/power.hpp"

namespace LayerTestsDefinitions {
    std::string PowerLayerTest::getTestCaseName(const testing::TestParamInfo<PowerParamsTuple> &obj) {
        std::vector<std::vector<size_t>> inputShapes;
        InferenceEngine::Precision netPrecision;
        std::string targetName;
        std::vector<float> power;
        std::tie(inputShapes, netPrecision, targetName, power) = obj.param;
        std::ostringstream results;

        results << "IS=" << CommonTestUtils::vec2str(inputShapes) << "_";
        results << "Power=" << CommonTestUtils::vec2str(power) << "_";
        results << "netPRC=" << netPrecision.name() << "_";
        results << "targetDevice=" << targetName << "_";
        return results.str();
    }

    void PowerLayerTest::SetUp() {
        threshold = 0.04f;

        std::vector<std::vector<size_t>> inputShapes;
        InferenceEngine::Precision netPrecision;
        std::vector<float> power;
        std::tie(inputShapes, netPrecision, targetDevice, power) = this->GetParam();
        auto ngPrc = FuncTestUtils::PrecisionUtils::convertIE2nGraphPrc(netPrecision);
        auto paramsIn = ngraph::builder::makeParams(ngPrc, {inputShapes[0]});

        auto power_const = std::make_shared<ngraph::op::Constant>(ngPrc, ngraph::Shape{ 1 }, power);
        auto pow = std::make_shared<ngraph::opset1::Power>(paramsIn[0], power_const);

        function = std::make_shared<ngraph::Function>(pow, paramsIn, "power");
    }

    TEST_P(PowerLayerTest, CompareWithRefs){
        Run();
    };
} // namespace LayerTestsDefinitions
