#include <cassert>
#include <iostream>
#ifndef FLATBUFFERS_DEBUG_VERIFICATION_FAILURE
#define FLATBUFFERS_DEBUG_VERIFICATION_FAILURE
#endif
#ifndef FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE
#define FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE
#endif

#include "Widget_generated.h"

using namespace flatbuffers;
using namespace widget;  // Specified in the schema.

int main(int argc, char *argv[]) {
  flatbuffers::FlatBufferBuilder builder(1024);

  // store Widget.list->A->B->C
  {
    auto cBuilder = CBuilder(builder);
    cBuilder.add_next_type(Object_NONE);
    cBuilder.add_c(2);
    auto c = cBuilder.Finish();

    auto bBuilder = BBuilder(builder);
    bBuilder.add_b(1);
    bBuilder.add_next_type(Object_C);
    bBuilder.add_next(c.Union());
    auto b = bBuilder.Finish();

    auto aBuilder = ABuilder(builder);
    aBuilder.add_a(0);
    aBuilder.add_next_type(Object_B);
    aBuilder.add_next(b.Union());
    auto a = aBuilder.Finish();

    auto widgetBuilder = WidgetBuilder(builder);
    widgetBuilder.add_list_type(Object_A);
    widgetBuilder.add_list(a.Union());
    auto widget = widgetBuilder.Finish();
    builder.Finish(widget);
  }

  flatbuffers::Verifier verifier(builder.GetBufferPointer(), builder.GetSize());
  assert(true == VerifyWidgetBuffer(verifier));

  // restore Widget.list->A->B->C
  auto widget = GetWidget(builder.GetBufferPointer());
  assert(widget->list_type() == Object_A);

  auto a = widget->list_as_A();
  assert(a->next_type() == Object_B);
  assert(a->a() == 0);

  auto b = a->next_as_B();
  assert(b->next_type() == Object_C);
  assert(b->b() == 1);

  auto c = b->next_as_C();
  assert(c->next_type() == Object_NONE);
  assert(c->c() == 2);

  std::cout << "DONE" << std::endl;

  return 0;
}
