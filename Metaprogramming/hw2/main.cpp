#include <iostream>
#include <typeinfo>
#include <climits>
#include "utils.h"
#include "typelist.h"
#include "factory.h"

struct Chair { Chair() { std::cout << "Chair\n"; } };
struct Table { Table() { std::cout << "Table\n"; } };
struct Sofa { Sofa() { std::cout << "Sofa\n"; } };

struct WoodChair : public Chair { WoodChair() { std::cout << "WoodChair\n"; } };
struct WoodTable : public Table { WoodTable() { std::cout << "WoodTable\n"; } };
struct WoodSofa: public Sofa { WoodSofa() { std::cout << "WoodSofa\n"; } };

struct MetalChair : public Chair { MetalChair() { std::cout << "MetalChair\n"; } };
struct MetalTable : public Table { MetalTable() { std::cout << "MetalTable\n"; } };
struct MetalSofa : public Sofa { MetalSofa() { std::cout << "MetalSofa\n"; } };

struct RussianMetalChair : public MetalChair { RussianMetalChair() { std::cout << "RussianMetalChair\n"; } };
struct RussianMetalTable : public MetalTable { RussianMetalTable() { std::cout << "RussianMetalTable\n"; } };
struct RussianMetalSofa : public MetalSofa { RussianMetalSofa() { std::cout << "RussianMetalSofa\n"; } };

int main() {

    typedef TypeList< TypeList<Chair, Table, Sofa>, TypeList<WoodChair, WoodTable, WoodSofa>,
            TypeList<MetalChair, MetalTable, MetalSofa>, TypeList<RussianMetalChair, RussianMetalTable, RussianMetalSofa>> typelist;
    auto MyFactoryHierarchy = GetAbstractFactory<typelist>();
    auto MyFactory = MyFactoryHierarchy.GetConcreteFactory<RussianMetalChair>();
    Sofa* a = MyFactory->Get<Sofa>();
    MetalSofa* b = MyFactory->Get<Sofa>();
    return 0;
}