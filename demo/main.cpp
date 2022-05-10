#include <boost/program_options.hpp>

#include "PageContainer.hpp"

namespace po = boost::program_options;

struct CmdArgs {
  std::string input;
  float threshold = 0;
};

bool parse_cmd(int argc, char* argv[], CmdArgs& args);

int main(int argc, char* argv[]) {
  CmdArgs args;
  if (!parse_cmd(argc, argv, args)) return 0;

  auto* used_memory = new UsedMemory;
  auto* sender = new StatSender;
  PageContainer page(sender, used_memory);

  std::ifstream f_in(args.input);
  page.Load(f_in, args.threshold);
  f_in.close();

  Log::get_instance().Write(std::to_string(used_memory->used()));

  for (size_t i = 0; i < 5; ++i) {
    const auto& item = page.ByIndex(i);
    std::cout << item.name << ": " << item.score << std::endl;
    const auto& item2 = page.ById(std::to_string(i));
    std::cout << item2.name << ": " << item2.score << std::endl;
  }

  page.Reload(args.threshold);
  Log::get_instance().Write(std::to_string(used_memory->used()));

  return 0;
}

bool parse_cmd(int argc, char* argv[], CmdArgs& args) {
  // Add options
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help, h", "produce help message")
      ("input",
       po::value<std::string>(&args.input)
           ->default_value("../tests/InputExample.txt"),
       "path/to/input/file")
      ("threshold",
       po::value<float>(&args.threshold)->default_value(0.1),
       "threshold value");

  // Parse arguments
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << "Usage: demo [options]" << std::endl;
    std::cout << desc << std::endl;
    return false;
  }

  return true;
}
