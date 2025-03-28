#include <Slartibartfast.hh>
#include <BonsaiProc.hh>
#include <GeoLinerHex.hh>
#include <GeoLiner.hh>

namespace SLARTIBARTFAST {
Slartibartfast::Slartibartfast(RAT::AnyParse *p, int argc, char **argv)
    : Rat(p, argc, argv) {
  // Append an additional data directory (for ratdb and geo)
  char *slartibartfastdata = getenv("SLARTIBARTFASTDATA");
  if (slartibartfastdata != NULL) {
    ratdb_directories.insert(static_cast<std::string>(slartibartfastdata) +
                             "/ratdb");
    model_directories.insert(static_cast<std::string>(slartibartfastdata) +
                             "/models");
  }
  // Initialize a geometry factory
  new GeoLinerHex();
  new GeoLiner();
  // Include a new type of processor
  RAT::ProcBlockManager::AppendProcessor<BonsaiProc>();
  // Add a unique component to the datastructure
}
} // namespace SLARTIBARTFAST
