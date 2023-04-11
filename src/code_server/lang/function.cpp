

#include "lang/function.hpp"
#include <string>
#include <vector>
#include <algorithm>

// In the form UDLR
typedef uint8_t ArrowIndentType;
enum {
    EMPTY,                  // 0000
    ARROW_RIGHT,            // 0001
    ARROW_LEFT,             // 0010
    ARROW_HORIZONTAL,       // 0011
    ARROW_DOWN,             // 0100
    ARROW_DOWN_RIGHT,       // 0101
    ARROW_DOWN_LEFT,        // 0110
    ARROW_T_UP,             // 0111
    ARROW_UP,               // 1000
    ARROW_UP_RIGHT,         // 1001
    ARROW_UP_LEFT,          // 1010
    ARROW_T_DOWN,           // 1011
    ARROW_VERTICAL,         // 1100
    ARROW_T_LEFT,           // 1101
    ARROW_T_RIGHT,          // 1110
    ARROW_ALL,              // 1111
    ARROW_HEAD,             // SPECIAL CASE
    ARROW_SELF,             // SPECIAL CASE
};

std::string ArrowIdentToStr(ArrowIndentType type) {
    switch (type)
    {
        case EMPTY:                 return " ";
        case ARROW_RIGHT:           return "╺";
        case ARROW_LEFT:            return "╸";
        case ARROW_UP:              return "╹";
        case ARROW_DOWN:            return "╻";
        case ARROW_DOWN_RIGHT:      return "┏";
        case ARROW_UP_RIGHT:        return "┗";
        case ARROW_HORIZONTAL:      return "━";
        case ARROW_VERTICAL:        return "┃";
        case ARROW_T_UP:            return "┳";
        case ARROW_T_DOWN:          return "┻";
        case ARROW_T_LEFT:          return "┣";
        case ARROW_T_RIGHT:         return "┫";
        case ARROW_ALL:             return "╋";
        case ARROW_HEAD:            return "►";
        case ARROW_SELF:            return "◊";
    
        default: return " ";
    }
}

class ArrowIndent
{
    std::vector<ArrowIndentType> indent_listing;
    bool input;
    bool output;
    bool self_ref;
public:
    ArrowIndent(): input(false), output(false), self_ref(false) {}

    void setInput(bool in) { input = in; }
    void setOutput(bool out) { output = out; }
    void setSelfRef(bool self) { self_ref = self; }

    void set(uint16_t index, ArrowIndentType type) {
        indent_listing.at(index) |= type;
    }

    void expand() {
        indent_listing.push_back(EMPTY);
    }

    // Builds the resulting indent
    std::string compile() {
        std::string out;
        for(uint16_t i = indent_listing.size(); i > 0; --i)
        {
            out.append(ArrowIdentToStr(indent_listing.at(i-1)));
        }
        if (self_ref) {
            out.append(ArrowIdentToStr(EMPTY) + ArrowIdentToStr(ARROW_SELF));
        }
        else {
            out.append(ArrowIdentToStr(input || output ? ARROW_HORIZONTAL : EMPTY) + ArrowIdentToStr(input ? ARROW_HEAD : (output ? ARROW_LEFT : EMPTY)));
        }
        return out;
    }
};

std::string discode::Function::repr()
{
    std::string s = "FUNCTION (";
    for(uint16_t i = 0; i < argnames.size(); ++i) {
        if (i) { s += ", "; }
        s += argnames.at(i);
    }
    return s + ")";
}

std::string discode::Function::deepRepr(std::string indentation)
{
    std::vector<std::string> out;
    std::vector<uint32_t> arrow_depths;
    std::vector<ArrowIndent> indent;
    uint32_t arrow_depth = 0;
    // Gather instructions
    for(uint32_t i = 0; i < instructions.size(); ++i)
    {
        Instruction * ins = instructions.at(i).get();
        std::string line_num = std::to_string(ins->linenum);
        while(line_num.size() < 3) line_num += " ";
        out.push_back(line_num + ": " + ins->repr());
        indent.push_back(ArrowIndent());
        arrow_depths.push_back(0);
    }
    out.push_back("");
    indent.push_back(ArrowIndent());
    arrow_depths.push_back(0);
    // Draw arrows
    for(uint32_t i = 0; i < instructions.size(); ++i)
    {
        Instruction * ins = instructions.at(i).get();
        if(ins->hasArrow())
        {
            uint32_t arrow_target = ins->arrow(i);
            if (arrow_target == i) {
                indent.at(i).setSelfRef(true);
                continue;
            }
            uint32_t max_depth = 0;
            // Determine arrow depth
            for(uint32_t j = std::min(i, arrow_target); j < std::max(i, arrow_target); ++j)
            {
                max_depth = std::max(arrow_depths.at(j), max_depth);
            }
            // Expand indent to fit arrows
            while (arrow_depth <= max_depth)
            {
                for(uint32_t j = 0; j < indent.size(); ++j)
                {
                    indent.at(j).expand();
                }
                ++arrow_depth;
            }
            // Draw arrow bar. Also, update the arrow depth chart
            uint32_t min = std::min(i, arrow_target);
            uint32_t max = std::max(i, arrow_target);
            for(uint32_t j = min; j <= max; ++j)
            {
                indent.at(j).set(max_depth, (j == min ? EMPTY : ARROW_UP) | (j == max ? EMPTY : ARROW_DOWN));
                arrow_depths.at(j) = max_depth + 1;
            }
            // Draw arrow head and tail
            for(uint32_t j = 0; j <= max_depth; ++j)
            {
                indent.at(arrow_target).setInput(true);
                indent.at(i).setOutput(true);

                indent.at(i).set(j, (j == max_depth ? ARROW_RIGHT : ARROW_HORIZONTAL));
                indent.at(arrow_target).set(j, (j == max_depth ? ARROW_RIGHT : ARROW_HORIZONTAL));
            }
        }
    }
    // Assemble final string
    // Function sig
    std::string s = repr() + "\n";
    // Instructions
    for(uint32_t i = 0; i < out.size(); ++i) {
        s.append(indentation);
        s.append(indent.at(i).compile());
        s.append(out.at(i));
        s.append("\n");
    }
    return s;
}

std::string discode::FunctionPtr::repr()
{
    if (!isComplete()) {
        return "F[" + std::to_string(ins_cnt) + "] -> " + current()->repr();
    }
    return "F[" + std::to_string(ins_cnt) + "] -> RET";
}
