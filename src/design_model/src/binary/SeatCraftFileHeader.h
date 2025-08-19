//
//  SeatCraftFileHeader.h
//  SeatCraftDesignModel
//
//  Created by king on 2025/8/19.
//

#ifndef SeatCraftFileHeader_h
#define SeatCraftFileHeader_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SEAT_CRAFT_MAGIC 0x83696584 /* the file magic number */
#define SEAT_CRAFT_CIGAM 0x84656983 /* NXSwapInt(SEAT_CRAFT_MAGIC) */

#pragma pack(push, 1)

struct SeatCraftFileHeader {
    uint32_t magic;
    uint16_t version;
    uint16_t reserved;
    uint32_t nSections;
};

enum SeatCraftSectionType : uint32_t {
    Section_Project = 1,     // 项目
    Section_Venue = 2,       // 场馆
    Section_FloatPool = 3,   // 浮点数池
    Section_ColorPool = 4,   // 颜色池
    Section_StringPool = 5,  // 字符串池 utf8
    Section_NodeTable = 6,
    Section_NodeData = 7,  // 节点数据
    Section_Extra = 0xFFFF
};

struct SeatCraftSectionHeader {
    SeatCraftSectionType type;  // Section 类型
    uint32_t offset;            // 文件内偏移
    uint32_t size;              // Section 数据长度（字节）
};

struct SeatCraftStringDesc {
    uint32_t index;
    uint32_t size;
};

struct SeatCraftProjectSectionData {
    SeatCraftStringDesc name;  // 项目名称对应字符串池索引
    SeatCraftStringDesc desc;  // 项目描述对应字符串池索引
    uint32_t nextVenueId;      // 下一个可用的场馆ID，同一个项目中唯一
    uint16_t nVenues;          // 场馆数量
    uint32_t extraOffset;      // 额外数据偏移
    uint32_t extraSize;        // 额外数据大小
};

struct SeatCraftVenueSectionData {
    SeatCraftStringDesc name;  // 场馆名称对应字符串池索引
    SeatCraftStringDesc desc;  // 场馆描述对应字符串池索引
    uint32_t nextNodeId;       // 下一个可用的节点ID，同一个场馆中唯一
    uint32_t nodeTableIndex;   // NodeTable 中的索引
    uint32_t extraOffset;      // 额外数据偏移
    uint32_t extraSize;        // 额外数据大小
};

struct SeatCraftNodeTableChunk {
    uint32_t offset;
    uint32_t size;
};

struct SeatCraftNodeTableData {
    uint32_t nums;
    SeatCraftNodeTableChunk *chunks;
};

typedef enum : uint16_t {
    SeatCraftGraphNodeType_Node = 1,      // 根类型
    SeatCraftGraphNodeType_Rect = 2,      // 矩形
    SeatCraftGraphNodeType_Triangle = 3,  // 三角形
} SeatCraftGraphNodeType;

struct SeatCraftGraphNode {
    SeatCraftGraphNodeType type;  // 节点类型
    uint32_t nodeId;              // 当前节点ID
    uint32_t parentId;            // 父节点ID
    uint32_t childCount;          // 子节点数量
    uint32_t childOffset;         // NodeTable 中子节点的偏移（相对当前Section或NodeTable起始）
    uint32_t extraOffset;         // 额外数据偏移
    uint32_t extraSize;           // 额外数据大小
};

struct SeatCraftGraphShapeNode {
    SeatCraftGraphNode base;
    uint32_t fillColorIndex;    // 对应颜色池的索引
    uint32_t strokeColorIndex;  // 对应颜色池的索引
    uint32_t strokeIndex;       // 对应浮点数池的索引
};

struct SeatCraftGraphRectNode {
    SeatCraftGraphShapeNode base;
    uint32_t xIndex;       // 对应浮点数池的索引
    uint32_t yIndex;       // 对应浮点数池的索引
    uint32_t widthIndex;   // 对应浮点数池的索引
    uint32_t heightIndex;  // 对应浮点数池的索引
};
#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* SeatCraftFileHeader_h */
