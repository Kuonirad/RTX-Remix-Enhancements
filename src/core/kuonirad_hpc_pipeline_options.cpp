#include "kuonirad_hpc_pipeline.h"

namespace rtx {

// Define static members with default values
namespace {
    static constexpr int DEFAULT_THREAD_COUNT = 8;
    static constexpr int DEFAULT_PDE_ITERATIONS = 10;
    static constexpr float DEFAULT_PDE_TIMESTEP = 0.1F;
}

const Option<bool> KuoniradHpcPipeline::enablePipeline(false);
const Option<int> KuoniradHpcPipeline::concurrencyThreads(DEFAULT_THREAD_COUNT);
const Option<PdeMethod> KuoniradHpcPipeline::pdeMethod(PdeMethod::ANISOTROPIC_DIFFUSION);
const Option<int> KuoniradHpcPipeline::pdeIterations(DEFAULT_PDE_ITERATIONS);
const Option<float> KuoniradHpcPipeline::pdeTimestep(DEFAULT_PDE_TIMESTEP);
const Option<bool> KuoniradHpcPipeline::useWavelet(true);
const Option<bool> KuoniradHpcPipeline::enableAiRefinement(true);

void KuoniradHpcPipeline::initialize() {
    // Nothing to initialize - static members are already initialized with defaults
}

} // namespace rtx
