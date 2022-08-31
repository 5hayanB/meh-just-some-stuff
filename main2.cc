#include "./tflite-micro/tensorflow/lite/micro/all_ops_resolver.h"
#include "./tflite-micro/tensorflow/lite/micro/micro_error_reporter.h"
#include "./tflite-micro/tensorflow/lite/schema/schema_generated.h"
#include "./tflite-micro/tensorflow/lite/micro/micro_interpreter.h"
#include "./yolov3.h"

class FlatBufferModel {
  // Build a model based on a file. Return a nullptr in case of failure.
  static std::unique_ptr<FlatBufferModel> BuildFromFile(
      const char* filename,
      ErrorReporter* error_reporter);

  // Build a model based on a pre-loaded flatbuffer. The caller retains
  // ownership of the buffer and should keep it alive until the returned object
  // is destroyed. Return a nullptr in case of failure.
  static std::unique_ptr<FlatBufferModel> BuildFromBuffer(
      const char* buffer,
      size_t buffer_size,
      ErrorReporter* error_reporter);
};

int main(void)
{
    // Load the model
    std::unique_ptr<tflite::FlatBufferModel> model =
        tflite::FlatBufferModel::BuildFromFile(filename);
    
    // Build the interpreter
    tflite::ops::builtin::BuiltinOpResolver resolver;
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::InterpreterBuilder(*model, resolver)(&interpreter);
    
    // Resize input tensors, if desired.
    interpreter->AllocateTensors();
    
    float* input = interpreter->typed_input_tensor<float>(0);
    // Fill `input`.
    
    interpreter->Invoke();
    
    float* output = interpreter->typed_output_tensor<float>(0);

    return 0;
}
