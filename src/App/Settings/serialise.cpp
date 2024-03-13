//
// Created by nathanial on 3/12/24.
//
#include <fstream>
#include "vector"
#include "string"
#include "unordered_map"
#include "functional"
#include "serialise.h"
#include "stack"

namespace Serialise {

    void Datafile::Set_String(const std::string &string, const size_t item) {
      if (item >= content.size())
        content.resize(item+1);

      content[item] = string;
    }

    std::string Datafile::Get_String(const size_t item) const {
      if (item >= content.size())
        return "";
      else
        return content[item];
    }

    void Datafile::Set_Real (const double d, const size_t item) {
      Set_String(std::to_string(d), item);
    }

    double Datafile::Get_Real (const size_t item) const {
      return std::strtof(Get_String(item).c_str(), nullptr);
    }

    void Datafile::Set_Int (const size_t i, const size_t item) {
      Set_String(std::to_string(i), item);
    }

    int32_t Datafile::Get_Int (const size_t item) const {
      return std::stoi(Get_String(item));
    }

    size_t Datafile::Get_Value_Count() const {
      return content.size();
    }

    size_t Datafile::Get_Map_Count() const {
      return mapObjects.size();
    }

    Datafile& Datafile::operator[](const std::string &name) {
      if (!mapObjects.contains(name)) {
        mapObjects[name] = vecObjects.size();
        vecObjects.emplace_back(name, Datafile());
      }
      return vecObjects[mapObjects[name]].second;
    }

    bool Datafile::Has_Property(const std::string &name) {
      return mapObjects.count(name);
    }

    Datafile& Datafile::Get_Property(const std::string &name) {
      size_t  x = name.find_first_of('.');
      if (x != std::string::npos) {
        std::string property = name.substr(0, x);
        if (Has_Property(property))
          return operator[](property).Get_Property((name.substr(x + 1, name.size())));
        else
          return operator[](property);
      }
      else {
        return operator[](name);
      }
    }

    Datafile Datafile::Get_Index_Property(const std::string &name, const size_t index) {
      return Get_Property(name + "[" + std::to_string(index) + "]");
    }

    bool Datafile::Write(const Datafile& data, const std::string &fileName, const std::string &indentStr, const char listSeperator) {
      std::string seperator = std::string(1, listSeperator) + " ";
      size_t indentCount = 0;

      std::function<void(const Datafile&, std::ofstream&)> write = [&](const Datafile &data, std::ofstream &file) {

//        alternative but will only take 1 char not a whole string
//        std::string(indentCount, '\n')
        auto applyIndent = [&](const std::string &string, const size_t count) {
          std::string out;
          for (int n = 0; n < count; n++) {
            out += string;
          }
          return out;
        };

        for (const auto &property : data.vecObjects) {
          if (property.second.vecObjects.empty()) {
            file << applyIndent(indentStr, indentCount) << property.first << (property.second.isComment ? "" : " = ");

            size_t items = property.second.Get_Value_Count();
            for (int i = 0; i < property.second.Get_Value_Count(); ++i) {
              size_t  x = property.second.Get_String(i).find_first_of(listSeperator);
              if (x != std::string::npos) {
                file << "\"" << property.second.Get_String(i) << "\"" << ((items > 1) ? seperator : "");
              }
              else {
                file << property.second.Get_String(i) << ((items > 1) ? seperator : "");
              }
              items--;
            }
            file << "\n";
          }
          else {
            file << applyIndent(indentStr, indentCount) << property.first << "\n";
            file << applyIndent(indentStr, indentCount) << "{\n";
            indentCount++;
            write(property.second, file);
            file << applyIndent(indentStr, indentCount) << "}\n";
          }
        }
        if (indentCount > 0) indentCount--;
      };

      std::ofstream file(fileName);
      if (file.is_open()) {
        write(data, file);
        return true;
      }
      return true;
    }

  bool Datafile::Read(Datafile &data, const std::string &fileName, const char listSeperator) {

    std::ifstream file(fileName);
    if (file.is_open())
    {
      std::string propertyName;
      std::string propertyValue;

      std::stack<std::reference_wrapper<Datafile>> stackPath;
      stackPath.emplace(data);

      while (!file.eof()) {

        std::string line;
        std::getline(file, line);

        auto trim = [](std::string& string) {
          string.erase(0, string.find_first_not_of("\t\n\r\f\v"));
          string.erase(string.find_last_not_of("\t\n\r\f\v") + 1);
        };

        trim(line);

        if (!line.empty()) {
          if (line[0] == '#') {
            Datafile comment;
            comment.isComment = true;
            stackPath.top().get().vecObjects.push_back({line, comment});
          }

          else {
            size_t x = line.find_first_of('=');
            if (x != std::string::npos) {
              propertyName = line.substr(0, x - 1);
//              propertyName = line.substr(0, x);
              trim(propertyName);

              propertyValue = line.substr(x + 2, line.size());
              trim(propertyValue);

              bool inQuotes;
              std::string token;
              size_t tokenCount = 0;
              for (const auto c: propertyValue) {
                if (c == '\"') {
                  inQuotes = !inQuotes;
                } else {
                  if (inQuotes) {
                    token.append(1, c);
                  } else {
                    if (c == listSeperator) {
                      trim(token);
                      stackPath.top().get()[propertyName].Set_String(token, tokenCount);
                      token.clear();
                      tokenCount++;
                    } else {
                      token.append(1, c);
                    }
                  }
                }
              }
              if (!token.empty()) {
                trim(token);
                stackPath.top().get()[propertyName].Set_String(token, tokenCount);
              }
            } else {
              if (line[0] == '{') {
                stackPath.push(stackPath.top().get()[propertyName]);
              } else {
                if (line[0] == '}') {
                  stackPath.pop();
                } else {
                  propertyName = line;
                }
              }
            }
          }
        }
      }

      file.close();
      return true;
    }
    return false;
  }
}
