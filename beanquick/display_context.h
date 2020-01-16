#ifndef DEANQUICK_DISPLAY_CONTEXT_H_
#define DEANQUICK_DISPLAY_CONTEXT_H_

namespace beanquick {

enum class DisplayPrecision {
  MOST_COMMON,
  MAXIMUM,
  NONE,
};

enum class DisplayAlignment {
  NATURAL,
  DOT,
  RIGHT,
  NONE,
};

class DisplayContext {
 public:
  DisplayContext();

  void Build(DisplayAlignment align, DisplayPrecision precision, commas = false,
             reserved = 0);

 private:
  std::unordere_map<string, CurrencyContext> ccontexts_;
};

}  // namespace beanquick

#endif  // DEANQUICK_DISPLAY_CONTEXT_H_
