#include "kuonirad_hpc_pipeline.h"

namespace rtx {

// Define static members with default values
Option<bool> KuoniradHpcPipeline::enablePipeline(false);
Option<int> KuoniradHpcPipeline::concurrencyThreads(8);
Option<PdeMethod> KuoniradHpcPipeline::pdeMethod(PdeMethod::ANISOTROPIC_DIFFUSION);
Option<int> KuoniradHpcPipeline::pdeIterations(10);
Option<float> KuoniradHpcPipeline::pdeTimestep(0.1f);
Option<bool> KuoniradHpcPipeline::useWavelet(true);
Option<bool> KuoniradHpcPipeline::enableAiRefinement(true);

void KuoniradHpcPipeline::initialize() {
    // Nothing to initialize - static members are already initialized with defaults
}

} // namespace rtx
