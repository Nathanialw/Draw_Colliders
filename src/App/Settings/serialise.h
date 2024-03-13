#pragma once
//
// Created by nathanial on 3/12/24.
//

#ifndef BOX2D_COLLIDERS_SERIALIZE_H
#define BOX2D_COLLIDERS_SERIALIZE_H

#endif //BOX2D_COLLIDERS_SERIALIZE_H
#include "string"
#include "unordered_map"
#include "functional"

namespace Serialise {
  class Datafile {

  private:
    std::vector<std::string> content;
    std::vector<std::pair<std::string, Datafile>> vecObjects;
    std::unordered_map<std::string, size_t> mapObjects;
  protected:
    bool isComment = false;
  public:
    void Set_String(const std::string &string, const size_t item = 0);
    std::string Get_String(const size_t item = 0) const;
    void Set_Real (const double d, const size_t item = 0);
    double Get_Real (const size_t item = 0) const;
    void Set_Int (const size_t i, const size_t item = 0);
    int32_t Get_Int (const size_t item = 0) const;
    size_t Get_Value_Count() const;
    size_t Get_Map_Count() const;
    Datafile& operator[](const std::string &name);
    bool Has_Property(const std::string &name);
    Datafile& Get_Property(const std::string &name);
    Datafile Get_Index_Property(const std::string &name, const size_t index);

    static bool Write(const Datafile& data, const std::string &fileName, const std::string &indentStr = "\t", const char listSeperator = ',');
    bool Read(Datafile &data, const std::string &fileName, const char listSeperator  = ',');
  };


};