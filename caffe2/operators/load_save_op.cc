#include "caffe2/operators/load_save_op.h"

namespace caffe2 {

template <>
void LoadOp<CPUContext>::SetCurrentDevice(BlobProto* proto) {
  if (proto->has_tensor()) {
    proto->mutable_tensor()->mutable_device_detail()->set_device_type(CPU);
  }
}

namespace {
REGISTER_CPU_OPERATOR(Load, LoadOp<CPUContext>);
REGISTER_CPU_OPERATOR(Save, SaveOp<CPUContext>);
REGISTER_CPU_OPERATOR(Snapshot, SnapshotOp<CPUContext>);

OPERATOR_SCHEMA(Load)
    .NumInputs(0, 1)
    .NumOutputs(1, INT_MAX)
    .SetDoc(R"DOC(
The Load operator loads a set of serialized blobs from a db. It takes no
input and [1, infinity) number of outputs, using the db keys to match the db
entries with the outputs.

If an input is passed, then it is assumed that that input blob is a
DBReader to load from, and we ignore the db and db_type arguments.

)DOC")
    .Arg(
        "absolute_path",
        "(int, default 0) if set, use the db path directly and do not prepend "
        "the current root folder of the workspace.")
    .Arg("db", "(string) the path to the db to load.")
    .Arg("db_type", "(string) the type of the db.")
    .Arg(
        "keep_device",
        "(int, default 0) if nonzero, the blobs are loaded into the device that "
        "is specified in the serialized BlobProto. Otherwise, the device will be "
        "set as the one that the Load operator is being run under.");

OPERATOR_SCHEMA(Save).NumInputs(1, INT_MAX).NumOutputs(0)
.SetDoc(R"DOC(
The Save operator saves a set of blobs to a db. It takes [1, infinity) number
of inputs and has no output. The contents of the inputs are written into the
db specified by the arguments.
)DOC")
.Arg("absolute_path",
     "(int, default 0) if set, use the db path directly and do not prepend "
     "the current root folder of the workspace.")
.Arg("db", "(string) the path to the db to load.")
.Arg("db_type", "(string) the type of the db.");

OPERATOR_SCHEMA(Snapshot).NumInputs(1, INT_MAX).NumOutputs(0)
.SetDoc(R"DOC(
The Snapshot operator is similar to the Save operator, but allows one to save
to db every few iterations, with a db name that is appended with the iteration
count. It takes [1, infinity) number of inputs and has no output. The first
input has to be a TensorCPU of type int and has size 1 (i.e. the iteration
counter). This is determined whether we need to do snapshotting.
)DOC")
.Arg("absolute_path",
     "(int, default 0) if set, use the db path directly and do not prepend "
     "the current root folder of the workspace.")
.Arg("db", "(string) a template string that one can combine with the "
           "iteration to create the final db name. For example, "
           "\"/home/lonestarr/checkpoint_%08d.db\"")
.Arg("db_type", "(string) the type of the db.")
.Arg("every", "(int, default 1) the snapshotting is carried out when "
              "(iter mod every) is zero.");

NO_GRADIENT(Load);
SHOULD_NOT_DO_GRADIENT(Save);
SHOULD_NOT_DO_GRADIENT(Snapshot);
}  // namespace
}  // namespace caffe2
