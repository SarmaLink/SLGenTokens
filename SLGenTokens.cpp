// https://github.com/SarmaLink/SLGenTokens
// Generate C++ soure file and JSON file
// with common UUID, random and static constants

#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <nlohmann/json.hpp>

using namespace std;
using namespace boost;

using json = nlohmann::json;

int find_missing_fields(json j, const char **fields)
{ // function for validation of config file
  for (const char **field = fields; *field; field++)
  {
    cout << *field << endl;
    if (j[*field].is_null())
    {
      std::cerr << *field << " missing in config json file." << endl;
      return 1;
    }
  }
  return 0;
}

int main(int argc, char *argv[])
{ // ########## M A I N #############
  json
      config_json, // defines number and types of C++ constants and JSON values
      output_json; // contains the same data as C++ file

  const char
      *default_config_json_file_name = "SLGenTokens_config.json",
      *js_output_json_file_name = "output_json_file_name",
      *js_output_cpp_file_name = "output_cpp_file_name",
      *js_output_include_file_name = "output_include_file_name",
      *js_header_guard = "header_guard",
      *js_key = "json_key",
      *js_val = "value",
      *c_type = "cpp_var_type",
      *c_name = "cpp_var_name",
      *length_define_name = "length_define_name",
      *sl_uuid_length_define_name = "uuid_length_define_name",
      *sl_comments = "comments",
      *const_char = "const char *",
      *const_unsigned_char = "const unsigned char",
      *sl_uuid = "uuid",
      *sl_binary = "binary",
      *sl_alphanumeric = "alphanumeric",
      *sl_length = "length",
      //      *c_length_appendix = "_length",
          *sl_static = "static",
      *sl_random = "random";

  const char *config_json_file_name;
  if (argc > 1)
    config_json_file_name = argv[1];
  else
    config_json_file_name = default_config_json_file_name;
  /*ifstream config_json_in_file(config_json_file_name);
  if (config_json_in_file.is_open())
  {
    config_json_in_file >> config_json;
    config_json_in_file.close();
  }
  else*/
  { // Generate default configuration values.
    config_json[js_output_json_file_name] = "SLGenTokens_output.json";
    config_json[js_output_cpp_file_name] = "src\\SLGenTokens_generated.cpp";
    config_json[js_output_include_file_name] = "include\\SLGenTokens_generated.h";
    config_json[js_header_guard] = "SL_GEN_TOKENS_GENERATED_H";

    config_json[sl_static] = {
        {{js_key, "model"}, {js_val, "Sarma"}, {c_type, const_char}, {c_name, "SLGenTokens_model"}},
        {{js_key, "version"}, {js_val, "1.0"}, {c_type, const_char}, {c_name, "SLGenTokens_version"}}};

    config_json[sl_random][sl_uuid] = {
        {{js_key, "uuid1"}, {c_type, const_char}, {c_name, "SLGenTokens_uuid1"}},
        {{js_key, "uuid2"}, {c_type, const_char}, {c_name, "SLGenTokens_uuid2"}}};

    config_json[sl_uuid_length_define_name] = "SL_GEN_TOKENS_UUID_LEN";

    config_json[sl_random][sl_binary] = {
        {{js_key, "secret1"}, {c_type, const_unsigned_char}, {c_name, "SLGenTokens_secret1"}, {sl_length, 0x40}, {length_define_name, "SL_GEN_TOKENS_SECRET1_LEN"}},
        {{js_key, "secret2"}, {c_type, const_unsigned_char}, {c_name, "SLGenTokens_secret2"}, {sl_length, 0x20}, {length_define_name, "SL_GEN_TOKENS_SECRET2_LEN"}}};

    config_json[sl_random][sl_alphanumeric] = {
        {{js_key, "token1"}, {c_type, const_char}, {c_name, "SLGenTokens_token1"}, {sl_length, 20}, {length_define_name, "SL_GEN_TOKENS_TOKEN1_LEN"}},
        {{js_key, "token2"}, {c_type, const_char}, {c_name, "SLGenTokens_token2"}, {sl_length, 10}, {length_define_name, "SL_GEN_TOKENS_TOKEN2_LEN"}}};

    config_json[sl_comments] = {
        "This file must be unique for every device.",
        "It can be generated using SLGenTokens program.",
        "https://github.com/SarmaLink/SLGenTokens"};

    ofstream config_json_out_file(config_json_file_name);
    config_json_out_file << std::setw(4) << config_json << std::endl;
    config_json_out_file.close();
  }

  //  const auto output_json_file_name = config_json[js_output_json_file_name];
  // if (output_json_file_name.is_null())
  //  const string output_json_file_name = config_json[js_output_json_file_name];
  //  if (config_json[js_output_json_file_name].is_null())
  //  {
  //    std::cerr << js_output_json_file_name << " missing in file " << config_json_file_name << endl;
  //    return 1;
  //  }
  //  const auto output_cpp_file_name = config_json[js_output_cpp_file_name];
  //  if (output_cpp_file_name.is_null())
  //  {
  //    std::cerr << js_output_cpp_file_name << _missing_in_file_ << config_json_file_name << endl;
  //    return 1;
  //  }

  const char *required_fields[] = {
      js_output_json_file_name,
      js_output_cpp_file_name,
      js_output_include_file_name,
      js_header_guard,
      sl_random,
      sl_static,
      sl_uuid_length_define_name,
      0};

  if (find_missing_fields(config_json, required_fields))
  {
    return 1;
  }

  //  for (const char **field = required_fields; *field; field++)
  //  {
  //    cout << *field << endl;
  //    if (config_json[*field].is_null())
  //    {
  //      std::cerr << *field << " missing in file " << config_json_file_name << endl;
  //      return 1;
  //    }
  //  }

  //cout << output_json_file_name << endl;
  //  cout << typeid(output_json_file_name)<< endl;

  // auto const output_json_file_name = config_json.find(js_output_json_file_name);
  //for (auto const &comment : *output_json_file_name)
  // {
  //   cout << comment << endl;
  // }
  //  cout << "***********************************" << endl;
  const string uuid_length_define_name = config_json[sl_uuid_length_define_name];
  const string output_cpp_file_name = config_json[js_output_cpp_file_name];

  std::ofstream output_cpp_file(output_cpp_file_name);
  if (output_cpp_file.fail())
  {
    std::cerr << "Couldn't open the file " << output_cpp_file_name << endl;
    std::cerr << "likely the destination folder does not exist." << endl;
    return 1;
  }

  const string output_include_file_name = config_json[js_output_include_file_name];

  std::ofstream output_include_file(output_include_file_name);
  if (output_include_file.fail())
  {
    std::cerr << "Couldn't open the file " << output_include_file_name << endl;
    std::cerr << "likely the destination folder does not exist." << endl;
    return 1;
  }

  const string header_guard = config_json[js_header_guard];
  output_include_file << "#ifndef " << header_guard << endl;
  output_include_file << "#define " << header_guard << endl
                      << endl;

  //cout << output_json_file_name << endl;
  //  auto itemmms = config_json[sl_comments][0];
  //cout << itemmms << endl;

  auto const comments = config_json.find(sl_comments);
  if (comments == config_json.end())
  {
    std::cerr << "comments not found";
  }
  else
  {
    {
      for (string comment : *comments)
        output_cpp_file << "// " << comment << endl;
    }
  }

  //  auto const random_blocks = config_json.find(sl_random);
  //  for (auto const &random_block : *random_blocks)
  //  {
  //random_block[]

  //  }
  auto const alphanumeric_blocks = config_json[sl_random][sl_alphanumeric];
  const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  std::random_device random_device;
  std::mt19937 generator(random_device());
  std::uniform_int_distribution<> distr(0, characters.size() - 1);

  if (alphanumeric_blocks.is_null())
  {
    std::cerr << sl_random << " -> " << sl_alphanumeric << " missing in file " << config_json_file_name << endl;
  }
  else
  {
    const char *alphanumeric_fields[] = {
        js_key,
        c_type,
        c_name,
        sl_length,
        length_define_name,
        0};
    for (auto const &alphanumeric_block : alphanumeric_blocks)
    {
      if (find_missing_fields(alphanumeric_block, alphanumeric_fields))
      {
        return 1;
      }

      //      for (const char **field = alphanumeric_fields; *field; field++)
      //      {
      //        //    cout << *field << endl;
      //        if (alphanumeric_block[*field].is_null())
      //        {
      //          std::cerr << *field << " missing" << endl;
      //          return 1;
      //        }
      //      }
      const string jsk = alphanumeric_block[js_key];
      const string ct = alphanumeric_block[c_type];
      const string cn = alphanumeric_block[c_name];
      const std::size_t length = alphanumeric_block[sl_length];
      const string def_name = alphanumeric_block[length_define_name];

      std::string random_string;

      for (std::size_t i = 0; i < length; ++i)
      {
        random_string += characters[distr(generator)];
      }
      output_json[jsk.c_str()] = random_string;

      output_cpp_file << ct << " " << cn << " = \"" << random_string << "\";" << endl;

      output_include_file << "extern " << ct << " " << cn << ";" << endl;
      output_include_file << "#define " << def_name << " " << length << endl
                          << endl;

      //  output_cpp_file << "const int " << cn << c_length_appendix << " = " << length << ";" << endl;

      //      cout << "js_key = " << jsk << endl
      //           << "c_type = " << ct << endl
      //           << "c_name = " << cn << endl
      //           << "sl_length = " << length << endl;
    }
  }

  auto const binary_blocks = config_json[sl_random][sl_binary];

  if (binary_blocks.is_null())
  {
    std::cerr << sl_random << " -> " << sl_binary << " missing in file " << config_json_file_name << endl;
  }
  else
  {
    //    const char *dymmy = "dummy";

    const char *binary_fields[] = {
        js_key,
        c_type,
        c_name,
        sl_length, // dymmy,
        length_define_name,
        0};
    std::uniform_int_distribution<> distro0255(0, 255);

    for (auto const &binary_block : binary_blocks)
    {
      if (find_missing_fields(binary_block, binary_fields))
      {
        return 1;
      }

      //    for (const char **field = binary_fields; *field; field++)
      //    {
      //      //    cout << *field << endl;
      //      if (binary_block[*field].is_null())
      //      {
      //        std::cerr << *field << " missing" << endl;
      //        return 1;
      //      }
      //    }

      const string jsk = binary_block[js_key];
      const string ct = binary_block[c_type];
      const string cn = binary_block[c_name];
      const std::size_t length = binary_block[sl_length];
      const string def_name = binary_block[length_define_name];

      //      unsigned char *p = new unsigned char[length];

      output_include_file << "extern " << ct << " " << cn << "[];" << endl;
      output_cpp_file << ct << " " << cn << "[] = {";
      std::stringstream ss;
      ss << std::hex;
      output_cpp_file << std::hex;
      for (std::size_t i = 0; i < length; i++)
      {
        int r = distro0255(generator);
        if (i > 0 && i < length)
          output_cpp_file << ", ";
        if (i % 16 == 0)
          output_cpp_file << endl
                          << "    ";
        output_cpp_file << "0x" << std::setw(2) << std::setfill('0') << r;
        //output_json[jsk].push_back(r);

        ss << std::setw(2) << std::setfill('0') << r;
      }

      //      cout << ss.str();
      //const string str =
      output_json[jsk.c_str()] = ss.str();

      //      for (int i(0); i < len; ++i)

      //    return ss.str();

      //      delete[] p;

      output_cpp_file << "};" << endl;
      output_cpp_file << std::dec;
      //      output_cpp_file << "const int " << cn << c_length_appendix << " = " << length << ";" << endl;
      output_include_file << "#define " << def_name << " " << length << endl
                          << endl;
    }
  }

  auto const uuid_blocks = config_json[sl_random][sl_uuid];

  if (uuid_blocks.is_null())
  {
    std::cerr << sl_random << " -> " << sl_uuid << " missing in file " << config_json_file_name << endl;
  }
  else
  {

    // const char *dymmy = "dummy";

    const char *uuid_fields[] = {
        js_key,
        c_type,
        c_name, //dymmy,
        0};
    uuids::random_generator uuid_generator;

    const int uuid_length = 36;
    for (auto const &uuid_block : uuid_blocks)
    {
      if (find_missing_fields(uuid_block, uuid_fields))
      {
        return 1;
      }

      const string jsk = uuid_block[js_key];
      const string ct = uuid_block[c_type];
      const string cn = uuid_block[c_name];

      uuids::uuid uuid = uuid_generator();
      const string uuid_str = uuids::to_string(uuid);
      if (uuid_str.length() != uuid_length)
      {
        cout << "UUID length mismatch" << endl;
        return 1;
      }

      output_json[jsk.c_str()] = uuid_str;

      output_cpp_file << ct << " " << cn << " = \"" << uuid_str << "\";" << endl;
      output_include_file << "extern " << ct << " " << cn << ";" << endl;
      //  j["uuid"] = uuids::to_string(uuid);
    }
    output_include_file << "#define " << uuid_length_define_name << " " << uuid_length << endl
                        << endl;
  }

  output_cpp_file.close();

  output_include_file << "#endif /* " << header_guard << " */" << endl;

  output_include_file.close();
  const string output_json_file_name = config_json[js_output_json_file_name];

  ofstream output_json_file(output_json_file_name);
  output_json_file << std::setw(4) << output_json << std::endl;
  output_json_file.close();

  //  output_json   js_output_json_file_name

  cout << "xxxxxxxxxxxxxxxxxxxxx" << endl;
  for (json::iterator it = config_json.begin(); it != config_json.end(); ++it)
  {
    std::cout << it.key() << " : " << it.value() << "\n";
  }

  json j;
  std::string sw_ver_str = "gipnochan";
  if (argc > 1)
  {
    sw_ver_str = argv[1];
  }
  j["sw_ver"] = sw_ver_str;

  const char task[] = "task";
  if (argc > 2)
  {
    j[task] = argv[2];
  }
  else
  {
    j[task] = task;
  }

  //  uuids::random_generator uuid_generator;
  //  uuids::uuid uuid = uuid_generator();
  //  j["uuid"] = uuids::to_string(uuid);

  std::cout << j.dump(2) << endl;

  std::ofstream src_file("src\\uuid_tokens.cpp");
  //  src_file << "const char *getUUID(void) { return \"" << uuid << "\"; }" << endl;
  src_file << "static const unsigned char xPollux[] = {" << endl;
  std::uniform_int_distribution<> distro0255(0, 255);
  for (int n = 0; n < 4; n++)
  {
    src_file << "    ";
    for (int m = 0; m < 16; m++)
    {
      src_file << distro0255(generator) << ", ";
    }
    src_file << endl;
  }
  src_file << "    0};" << endl;
  src_file << "void xBuf(char *buf, const int len) { for (int n = 0; n < len; n++) buf[n] ^= xPollux[n & 0x3F]; }" << endl;
  src_file << "const unsigned char *Pollux(const int &n) { return &xPollux[n + 36]; }" << endl;
  src_file << "const char *get_sl_sw_ver(void) { return \"" << sw_ver_str << "\"; }" << endl;
  // src_file << "const char *get_rhythmic(void) { return \"" << random_string << "\"; }" << endl;

  /*
void xBuf(char *buf, const int len) { for (int n = 0; n < len; n++) buf[n] ^= xPollux[n & 0x3F]; }
const unsigned char *Pollux(const int &n) { return &xPollux[n + 36]; }
const char *get_sl_sw_ver(void) { return "0.707"; }
const char *get_rhythmic(void) { return "QdBljqAQgepSTYKQEtxr"; }

*/

  src_file.close();
  return 0;
}