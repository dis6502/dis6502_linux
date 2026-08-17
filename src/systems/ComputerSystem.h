#pragma once

#include "../Syntax.h"
#include "../Byte.h"
#include "../FileIO.h"
#include "../FileType.h"
#include "../FileHeader.h"
#include "../Memory.h"
#include "../SegmentTypes.h"
#include "ComputerFont.h"
#include "ComputerSystemType.h"
#include <gsl/pointers>
#include <vector>

class SegmentList;
class SegmentListInserter;
enum class FileType;
class InputStream;
class OutputStream;


class ComputerSystem;

class ComputerSystem {
public:

	/// Constructor and Init() are only called by the factory
	ComputerSystem(const ComputerSystem& a) = delete;			// no copy constructor
	ComputerSystem& operator=(const ComputerSystem&) = delete;  // no copy assignment
	ComputerSystem(ComputerSystem&&) = delete;                  // no move constructor
	ComputerSystem& operator=(ComputerSystem&&) = delete;       // no move assignment

    virtual ~ComputerSystem();

	/*
	** Get the type of the computer system.
	*/
	ComputerSystemType GetType() const;

	/**
	 * Get the info for the computer system type.
	 */
    gsl::not_null<const ComputerSystemTypeInfo*> GetTypeInfo() const; // TODO: Move const before not null

    /*
     * Get the file path of a resource file in the system folder based on the file name.
    **/
    wstring GetResourceFilePath(wstring_view fileName) const;

	/*
	 * Get the file path of a resource file in the system folder based on the file extension.
	**/
	wstring GetResourceFilePathByExtension(wstring_view extension) const;

	/*
	** Get the font of the computer system, optinally in double height.
	*/
	HFONT GetFont(bool doubleHeight = false) const;

	/*
	** Get return character which will be used to indicate line ends in strings.
	*/
	byte GetReturnCharacter() const;

	virtual bool IsBaseAddress(const Memory::address wAddr) const  = 0;

    /**
    **
    The IsVectorAddress() is also used in code trace. This is the way the program finds LO_BYTE and HI_BYTE pattern like this:

    lda #$34
    sta VKEYBD
    lda #$12
    sta VKEYBD+1

    If such a pattern is found code and IsVectorAddress(VKEYBD) == true then
    - $1234 is considered an address
    - a label "L1234" is created for it
    - the lda #$34 is changed to to lda #<L1234
    - the lda #$12 is changed to to lda #>L1234

    This is exactly the same as setting LO_BYTE and HI_BYTE but in an automatic way through code trace
    */
	virtual bool IsVectorAddress(const Memory::address wAddr) const  = 0;

	/*
	** Returns true if the address is a display list pointer address.
    *  The default implementation returns false.
	*/
	virtual bool IsDisplayListVectorAddress(const Memory::address wAddr) const;

	/*
	** Returns the best matching guess for the file type based on the file name, size and content.
	*/
	FileType GuessFileType(wstring_view filePath) const; // throws IOException

	/*
	** Returns the mast matching guess for the file type based on the file size and content.
	*  The byte array passed has exactly 4 bytes length.
	*/
	virtual FileType GuessFileType(const FileIO::FILE_SIZE fileSize, ByteSequence& content) const = 0;

	/*
	** Retruns true if the file type is supported.
	*/
	bool IsSupportedFileType(const FileType fileType) const;

	/*
	** Read a file and add one or more segments to the segment list.
    *  Can throw FileIO::IOException in case of errors.
	*/
	virtual void ReadFile(const FileType fileType, InputStream& inputStream, FileIO::FILE_SIZE fileSize, SegmentListInserter& segmentListInserter) const;

	/*
	** Write a segment or all segments (if firstSegmentIndex = NO_SEGMENT_INDEX) to an executable file.
	*/
	virtual void WriteExecutableFile(const SegmentList& segmentList, const SEGMENT_INDEX firstSegmentIndex, const bool bWriteHeader, OutputStream& outputStream) const; // throws IOException


protected:

	ComputerSystem(const ComputerSystemTypeInfo& computerSystemTypeInfo);

	/*
	** Read a cassette file.
	*/
	virtual void ReadCassetteFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSiz) const;

	/*
	** Read an executable file.
	*/
	virtual void ReadExecutableFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const;

	/*
	** Read a ROM file.
	*/
	virtual void ReadROMFile(SegmentListInserter& segmentListInserter, InputStream& inputStream, const FileIO::FILE_SIZE fileSize) const;

protected:
	gsl::not_null<const ComputerSystemTypeInfo*> computerSystemTypeInfo;
	byte returnCharacter;
	std::vector<FileType> supportedFileTypes;

private:
    friend class ComputerSystemFactory;

	wstring fontFilePath;
	std::unique_ptr<ComputerFont> computerFont;
    void Init(); 
};
