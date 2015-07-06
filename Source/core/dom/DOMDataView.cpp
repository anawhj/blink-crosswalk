// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"
#include "core/dom/DOMDataView.h"

#include "bindings/core/v8/DOMDataStore.h"
#include "bindings/core/v8/V8ArrayBuffer.h"
#include "bindings/core/v8/V8DOMWrapper.h"
#include "platform/CheckedInt.h"
#include "wtf/ArrayBufferView.h"

namespace blink {

namespace {

class DataView final : public ArrayBufferView {
public:
    static PassRefPtr<DataView> create(PassRefPtr<ArrayBuffer> buffer, unsigned byteOffset, unsigned byteLength)
    {
        RELEASE_ASSERT(byteOffset <= buffer->byteLength());
        CheckedInt<uint32_t> checkedOffset(byteOffset);
        CheckedInt<uint32_t> checkedLength(byteLength);
        CheckedInt<uint32_t> checkedMax = checkedOffset + checkedLength;
        RELEASE_ASSERT(checkedMax.isValid());
        RELEASE_ASSERT(checkedMax.value() <= buffer->byteLength());
        return adoptRef(new DataView(buffer, byteOffset, byteLength));
    }

    virtual unsigned byteLength() const override { return m_byteLength; }
    virtual ViewType type() const override { return TypeDataView; }

protected:
    virtual void neuter() override
    {
        ArrayBufferView::neuter();
        m_byteLength = 0;
    }

private:
    DataView(PassRefPtr<ArrayBuffer> buffer, unsigned byteOffset, unsigned byteLength)
        : ArrayBufferView(buffer, byteOffset)
        , m_byteLength(byteLength) { }

    unsigned m_byteLength;
};

} // anonymous namespace

PassRefPtr<DOMDataView> DOMDataView::create(PassRefPtr<DOMArrayBufferBase> prpBuffer, unsigned byteOffset, unsigned byteLength)
{
    RefPtr<DOMArrayBufferBase> buffer = prpBuffer;
    RefPtr<DataView> dataView = DataView::create(buffer->buffer(), byteOffset, byteLength);
    return adoptRef(new DOMDataView(dataView.release(), buffer.release()));
}

v8::Local<v8::Object> DOMDataView::wrap(v8::Isolate* isolate, v8::Local<v8::Object> creationContext)
{
    // It's possible that no one except for the new wrapper owns this object at
    // this moment, so we have to prevent GC to collect this object until the
    // object gets associated with the wrapper.
    RefPtr<DOMDataView> protect(this);

    ASSERT(!DOMDataStore::containsWrapper(this, isolate));

    const WrapperTypeInfo* wrapperTypeInfo = this->wrapperTypeInfo();
    v8::Local<v8::Value> v8Buffer = toV8(buffer(), creationContext, isolate);
    if (v8Buffer.IsEmpty())
        return v8::Handle<v8::Object>();
    ASSERT(v8Buffer->IsArrayBuffer());

    v8::Local<v8::Object> wrapper = v8::DataView::New(v8Buffer.As<v8::ArrayBuffer>(), byteOffset(), byteLength());
    // V8::DataView::New may run an arbitrary script and it may result in
    // creating a new wrapper and associating it with |this|.  If so, the
    // wrapper already created and associated must be used.
    v8::Local<v8::Object> associatedWrapper = DOMDataStore::getWrapper(this, isolate);
    if (UNLIKELY(!associatedWrapper.IsEmpty()))
        return associatedWrapper;

    return associateWithWrapper(isolate, wrapperTypeInfo, wrapper);
}

v8::Local<v8::Object> DOMDataView::associateWithWrapper(v8::Isolate* isolate, const WrapperTypeInfo* wrapperTypeInfo, v8::Local<v8::Object> wrapper)
{
    return V8DOMWrapper::associateObjectWithWrapper(isolate, this, wrapperTypeInfo, wrapper);
}

} // namespace blink
