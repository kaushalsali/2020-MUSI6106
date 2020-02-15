#if !defined(__RingBuffer_hdr__)
#define __RingBuffer_hdr__

#include <cassert>
#include <algorithm>
#include <cmath>

/*! \brief implement a circular buffer of type T
*/
template <class T>
class CRingBuffer
{
public:
    explicit CRingBuffer (int iBufferLengthInSamples) :
        m_iBuffLength(iBufferLengthInSamples),
        currentBufferLength(iBufferLengthInSamples),
        read(0),
        write(0),
        empty(true),
        full(false)
    {
        assert(iBufferLengthInSamples > 0);

        // allocate and init
        buffer = new T[iBufferLengthInSamples];
    }

    virtual ~CRingBuffer ()
    {
        // free memory
        delete[] buffer;
        buffer = nullptr;
    }

    /*! add a new value of type T to write index and increment write index
    \param tNewValue the new value
    \return void
    */
    void putPostInc (T tNewValue)
    {
        if (!full) {
            put(tNewValue);
            write = (write + 1) % currentBufferLength;
            if (empty)
                empty = false;
            if (read == write)
                full = true;
        }
        else {
            std::cout << "Buffer Full" << std::endl; //TODO: Throw error instead
        }
    }

    /*! add new values of type T to write index and increment write index
    \param ptNewBuff: new values
    \param iLength: number of values
    \return void
    */
    void putPostInc (const T* ptNewBuff, int& iLength) //At the end iLength will contain number of samples that were actually inserted.
    {
        if (!full) {
            for (int i = 0; i < iLength; i++) {
                buffer[write] = ptNewBuff[i];
                write = (write + 1) % currentBufferLength;
                if (empty)
                    empty = false;
                if (read == write) {
                    full = true;
                    iLength = i;
                    break;
                }
            }
        }
        else
            iLength = 0;
    }

    /*! add a new value of type T to write index
    \param tNewValue the new value
    \return void
    */
    void put(T tNewValue)  // CAUTION: Overwrite possible
    {
        buffer[write] = tNewValue;
    }

    /*! add new values of type T to write index
    \param ptNewBuff: new values
    \param iLength: number of values
    \return void
    */
    void put(const T* ptNewBuff, int iLength) // CAUTION: Overwrite possible
    {
        for (int i=0; i<iLength; i++)
            buffer[(write + i) % currentBufferLength ] = ptNewBuff[i];
    }

    /*! return the value at the current read index and increment the read pointer
    \return float the value from the read index
    */
    T getPostInc ()
    {
        if (!empty) {
            T sample = buffer[read];
            read = (read + 1) % currentBufferLength;
            if (full)
                full = false;
            if (read == write)
                empty = true;
            return sample;
        }
        else
            std::cout<< "Buffer Empty" << std::endl; // TODO: Throw Error or something
    }

    /*! return the values starting at the current read index and increment the read pointer
    \param ptBuff: pointer to where the values will be written
    \param iLength: number of values
    \return void
    */
    void getPostInc (T* ptBuff, int& iLength)
    {
        if (!empty) {
            for (int i = 0; i < iLength; i++) {
                ptBuff[i] = buffer[read];
                read = (read + 1) % currentBufferLength;
                if (full)
                    full = false;
                if (read == write) {
                    empty = true;
                    iLength = i;
                    break;
                }
            }
        }
        else
            iLength = 0;
    }

    /*! return the value at the current read index
    \param fOffset: read at offset from read index
    \return float the value from the read index
    */
    T get (float fOffset = 0.f) const
    {
        float fracPart, intPart;
        fracPart = modf(fOffset, &intPart);
        T x = buffer[(int)(read + intPart) % currentBufferLength];
        if (fracPart == 0.f)
            return x;
        else {
            T y = buffer[(int)(read + intPart + (fracPart / abs(fracPart))) % currentBufferLength];
            return (1 - abs(fracPart)) * x + abs(fracPart) * y;
        }
    }

    /*! return the values starting at the current read index
    \param ptBuff to where the values will be written
    \param iLength: number of values
    \return void
    */
    void get (T* ptBuff, int iLength) const
    {
        for (int i=0; i<iLength; i++)
            ptBuff[i] = buffer[(read + i) % currentBufferLength];
    }

    /*! set buffer content and indices to 0
    \return void
    */
    void reset ()
    {
        for (int i=0; i<m_iBuffLength; i++) {
            buffer[i] = 0;
        }
        read = 0;
        write = 0;
        empty = true;
        full = false;
    }

    /*! return the current index for writing/put
    \return int
    */
    int getWriteIdx () const
    {
        return -1;
    }

    /*! move the write index to a new position
    \param iNewWriteIdx: new position
    \return void
    */
    void setWriteIdx (int iNewWriteIdx)
    {
    }

    /*! return the current index for reading/get
    \return int
    */
    int getReadIdx () const
    {
        return -1;
    }

    /*! move the read index to a new position
    \param iNewReadIdx: new position
    \return void
    */
    void setReadIdx (int iNewReadIdx)
    {
    }

    /*! returns the number of values currently buffered (note: 0 could also mean the buffer is full!)
    \return int
    */
    int getNumValuesInBuffer () const
    {
        return -1;
    }

    /*! returns the length of the internal buffer
    \return int
    */
    int getLength () const
    {
        return -1;
    }

    void show() {
        for (int i=0; i<m_iBuffLength; i++) {
            if (i == read)
                std::cout << "(r)";
            if (i == write)
                std::cout << "(w)";

            std::cout << buffer[i] << "--";
        }
        std::cout << std::endl;
        std::cout << "read: " << read << "  write: " << write << std::endl;
    }

private:
    CRingBuffer ();
    CRingBuffer(const CRingBuffer& that);

    int m_iBuffLength;              //!< length of the internal buffer
    int currentBufferLength;

    T *buffer;
    int read;
    int write;
    bool empty;
    bool full;
};
#endif // __RingBuffer_hdr__




