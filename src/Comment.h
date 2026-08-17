#pragma once

#include "Syntax.h"
#include "Memory.h"
#include "XML.h"

#include <vector>
#include <gsl/gsl>

class Comment final : public XML::Serializable {
public:

	Comment();
	~Comment();

	void Clear();
	Memory::offset GetOffset() const;
	void SetOffset(Memory::offset offset);
	wstring GetText() const;
	void SetText(wstring_view text);

	void SerializeTo(XML::Element& element) const override;
	void DeserializeFrom(const XML::Element& element) override;

private:
	Memory::offset offset;  // memory offset of the byte owning the comment
	wstring text;       // text of the comment (can be multi-line)
};

typedef std::vector<gsl::not_null<Comment*>> CommentList;
