// https://github.com/SarmaLink/SLGenTokens
// Generate C++ soure file with header and JSON file
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

int detect_missing_fields(json j, const char **fields)
{ // for validation of config file
  for (const char **field = fields; *field; field++)
  {
    if (j[*field].is_null())
    {
      cerr << *field << " missing in config json file." << endl;
      return 1;
    }
  }
  return 0;
}

int main(int argc, char *argv[])
{
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
      *sl_static = "static",
      *sl_json_only = "json_only",
      *sl_random = "random";

  const char *config_json_file_name;
  if (argc > 1)
    config_json_file_name = argv[1];
  else
    config_json_file_name = default_config_json_file_name;
  ifstream config_json_in_file(config_json_file_name);
  if (config_json_in_file.is_open())
  {
    config_json_in_file >> config_json;
    config_json_in_file.close();
  }
  else
  { // Generate default configuration values.
    config_json[js_output_json_file_name] = "SLGenTokens_output.json";
    config_json[js_output_cpp_file_name] = "src\\SLGenTokens_generated.cpp";
    config_json[js_output_include_file_name] = "include\\SLGenTokens_generated.h";
    config_json[js_header_guard] = "SL_GEN_TOKENS_GENERATED_H";

    config_json[sl_static] = {
        {{js_key, "model"}, {js_val, "Sarma"}, {c_type, const_char}, {c_name, "SLGenTokens_model"}, {length_define_name, "SL_GEN_TOKENS_MODEL_LEN"}},
        {{js_key, "version"}, {js_val, "1.0"}, {c_type, const_char}, {c_name, "SLGenTokens_version"}, {length_define_name, "SL_GEN_TOKENS_VERSION_LEN"}}};

    config_json[sl_json_only] = {
        {{js_key, "message1"}, {js_val, "test1"}},
        {{js_key, "message2"}, {js_val, "test2"}}};

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
    config_json_out_file << setw(4) << config_json << endl;
    config_json_out_file.close();
  }

  const char *required_fields[] = {
      js_output_json_file_name,
      js_output_cpp_file_name,
      js_output_include_file_name,
      js_header_guard,
      sl_random,
      sl_static,
      sl_json_only,
      sl_uuid_length_define_name,
      0};

  if (detect_missing_fields(config_json, required_fields))
  {
    return 1;
  }

  const string uuid_length_define_name = config_json[sl_uuid_length_define_name];
  const string output_cpp_file_name = config_json[js_output_cpp_file_name];

  ofstream output_cpp_file(output_cpp_file_name);
  if (output_cpp_file.fail())
  {
    cerr << "Couldn't open the file " << output_cpp_file_name << endl;
    cerr << "likely the destination folder does not exist." << endl;
    return 1;
  }

  const string output_include_file_name = config_json[js_output_include_file_name];

  ofstream output_include_file(output_include_file_name);
  if (output_include_file.fail())
  {
    cerr << "Couldn't open the file " << output_include_file_name << endl;
    cerr << "likely the destination folder does not exist." << endl;
    return 1;
  }

  const string header_guard = config_json[js_header_guard];
  output_include_file << "#ifndef " << header_guard << endl;
  output_include_file << "#define " << header_guard << endl
                      << endl;

  {
    // #########################################################
    // ###################### sl_comments ######################
    // #########################################################
    auto const comments = config_json.find(sl_comments);
    if (comments == config_json.end())
    {
      cerr << "comments not found";
    }
    else
    {
      {
        for (string comment : *comments)
          output_cpp_file << "// " << comment << endl;
      }
    }
  }

  random_device random_device;
  std::mt19937 generator(random_device());

  {
    // #########################################################
    // ###################### sl_alphanumeric ##################
    // #########################################################

    auto const alphanumeric_blocks = config_json[sl_random][sl_alphanumeric];
    const string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    uniform_int_distribution<> distr(0, characters.size() - 1);

    if (alphanumeric_blocks.is_null())
    {
      cerr << sl_random << " -> " << sl_alphanumeric << " missing in file " << config_json_file_name << endl;
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
        if (detect_missing_fields(alphanumeric_block, alphanumeric_fields))
        {
          return 1;
        }

        const string jsk = alphanumeric_block[js_key];
        const string ct = alphanumeric_block[c_type];
        const string cn = alphanumeric_block[c_name];
        const size_t length = alphanumeric_block[sl_length];
        const string def_name = alphanumeric_block[length_define_name];

        string random_string;

        for (size_t i = 0; i < length; ++i)
        {
          random_string += characters[distr(generator)];
        }
        output_json[jsk.c_str()] = random_string;

        output_cpp_file << ct << " " << cn << " = \"" << random_string << "\";" << endl;

        output_include_file << "extern " << ct << " " << cn << ";" << endl;
        output_include_file << "#define " << def_name << " " << length << endl
                            << endl;
      }
    }
  }

  {
    // #########################################################
    // ###################### sl_binary ########################
    // #########################################################

    auto const binary_blocks = config_json[sl_random][sl_binary];

    if (binary_blocks.is_null())
    {
      cerr << sl_random << " -> " << sl_binary << " missing in file " << config_json_file_name << endl;
    }
    else
    {
      const char *binary_fields[] = {
          js_key,
          c_type,
          c_name,
          sl_length,
          length_define_name,
          0};
      uniform_int_distribution<> distro0255(0, 255);

      for (auto const &binary_block : binary_blocks)
      {
        if (detect_missing_fields(binary_block, binary_fields))
        {
          return 1;
        }

        const string jsk = binary_block[js_key];
        const string ct = binary_block[c_type];
        const string cn = binary_block[c_name];
        const size_t length = binary_block[sl_length];
        const string def_name = binary_block[length_define_name];

        output_include_file << "extern " << ct << " " << cn << "[];" << endl;
        output_cpp_file << ct << " " << cn << "[] = {";
        stringstream ss;
        ss << hex;
        output_cpp_file << hex;
        for (size_t i = 0; i < length; i++)
        {
          int r = distro0255(generator);
          if (i > 0 && i < length)
            output_cpp_file << ", ";
          if (i % 16 == 0)
            output_cpp_file << endl
                            << "    ";
          output_cpp_file << "0x" << setw(2) << setfill('0') << r;

          ss << setw(2) << setfill('0') << r;
        }

        output_json[jsk.c_str()] = ss.str();

        output_cpp_file << "};" << endl;
        output_cpp_file << dec;
        output_include_file << "#define " << def_name << " " << length << endl
                            << endl;
      }
    }
  }
  {
    // #########################################################
    // ######################## sl_uuid ########################
    // #########################################################
    auto const uuid_blocks = config_json[sl_random][sl_uuid];

    if (uuid_blocks.is_null())
    {
      cerr << sl_random << " -> " << sl_uuid << " missing in file " << config_json_file_name << endl;
    }
    else
    {

      const char *uuid_fields[] = {
          js_key,
          c_type,
          c_name,
          0};
      uuids::random_generator uuid_generator;

      const int uuid_length = 36;
      for (auto const &uuid_block : uuid_blocks)
      {
        if (detect_missing_fields(uuid_block, uuid_fields))
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
      }
      output_include_file << "#define " << uuid_length_define_name << " " << uuid_length << endl
                          << endl;
    }
  }
  {
    // #########################################################
    // ###################### sl_static ########################
    // #########################################################

    auto const static_blocks = config_json[sl_static];

    if (static_blocks.is_null())
    {
      cerr << sl_static << " missing in file " << config_json_file_name << endl;
    }
    else
    {
      const char *static_fields[] = {
          js_key,
          js_val,
          c_type,
          c_name,
          length_define_name,
          0};
      for (auto const &static_block : static_blocks)
      {
        if (detect_missing_fields(static_block, static_fields))
        {
          return 1;
        }
        const string jsk = static_block[js_key];
        const string val = static_block[js_val];
        const string ct = static_block[c_type];
        const string cn = static_block[c_name];
        const size_t length = val.length();
        const string def_name = static_block[length_define_name];
        output_json[jsk.c_str()] = val;
        output_cpp_file << ct << " " << cn << " = \"" << val << "\";" << endl;
        output_include_file << "extern " << ct << " " << cn << ";" << endl;
        output_include_file << "#define " << def_name << " " << length << endl
                            << endl;
      }
    }
  }

  {
    // #########################################################
    // ###################### sl_json_only #####################
    // #########################################################
    auto const json_only_blocks = config_json[sl_json_only];

    if (json_only_blocks.is_null())
    {
      cerr << sl_static << " missing in file " << config_json_file_name << endl;
    }
    else
    {
      const char *json_only_fields[] = {
          js_key,
          js_val,
          0};
      for (auto const &json_only_block : json_only_blocks)
      {
        if (detect_missing_fields(json_only_block, json_only_fields))
        {
          return 1;
        }
        const string jsk = json_only_block[js_key];
        const string val = json_only_block[js_val];

        output_json[jsk.c_str()] = val;
      }
    }
  }

  output_cpp_file.close();

  output_include_file << "#endif /* " << header_guard << " */" << endl;

  output_include_file.close();
  const string output_json_file_name = config_json[js_output_json_file_name];

  ofstream output_json_file(output_json_file_name);
  output_json_file << setw(4) << output_json << endl;
  output_json_file.close();

  return 0;
}