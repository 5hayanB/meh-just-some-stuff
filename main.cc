#include "./tflite-micro/tensorflow/lite/micro/all_ops_resolver.h"
#include "./tflite-micro/tensorflow/lite/micro/micro_error_reporter.h"
#include "./tflite-micro/tensorflow/lite/schema/schema_generated.h"
#include "./tflite-micro/tensorflow/lite/micro/micro_interpreter.h"
#include "./yolov3.h"

namespace
{
    tflite::ErrorReporter* error_reporter = nullptr;
    const tflite::Model* model = nullptr;
    tflite::MicroInterpreter* interpreter = nullptr;
    TfLiteTensor* input = nullptr;
    TfLiteTensor* output = nullptr;
    constexpr int kTensorArenaSize = 100 * 1024;
    static uint8_t tensor_arena[kTensorArenaSize];

    // Functions
    TfLiteStatus GetImage(
        tflite::ErrorReporter* error_reporter,
        int width,
        int height,
        int channels,
        int8_t* data,
        const uint8_t* image,
        size_t bytes
    ){
        memcpy(data, image, bytes);
        return kTfLiteOk;
    }
}  // namespace

void setup(void)
{
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    model = tflite::GetModel(yolov3_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        error_reporter->Report(
            "Model schema %d is not equal to supported schema version %d",
            model->version(),
            TFLITE_SCHEMA_VERSION
        );
        return;
    }
    static tflite::AllOpsResolver all_ops_resolver;

    static tflite::MicroInterpreter static_interpreter(
        model,
        all_ops_resolver,
        tensor_arena,
        kTensorArenaSize,
        error_reporter
    );
    interpreter = &static_interpreter;
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk)
    {
        TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
        return;
    }
    input = interpreter->input(0);
}

void infer(void)
{
    if (kTfLiteOk != GetImage(
        error_reporter,
        image_width,
        image_height,
        channels,
        input->data.int8,
        image,
        input->bytes
    )){
        TF_LITE_REPORT_ERROR(error_reporter, "GetImage() failed");
    }

    if (kTfLiteOk != interpreter->Invoke())
    {
        TF_LITE_REPORT_ERROR(error_reporter, "Invoke() failed");
    }

    output = interpreter->output(0);
}

int main(void)
{
    printf("Initializing setup");
    setup();
    printf("Model has been ");
    infer();

    return 0;
}
