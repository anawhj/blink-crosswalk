// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef TransformDisplayItem_h
#define TransformDisplayItem_h

#include "platform/graphics/paint/DisplayItem.h"
#include "platform/transforms/AffineTransform.h"
#include "wtf/PassOwnPtr.h"

namespace blink {

class PLATFORM_EXPORT BeginTransformDisplayItem : public PairedBeginDisplayItem {
public:
    BeginTransformDisplayItem(const DisplayItemClientWrapper& client, const AffineTransform& transform)
        : PairedBeginDisplayItem(client, BeginTransform)
        , m_transform(transform) { }

    virtual void replay(GraphicsContext&) override;
    virtual void appendToWebDisplayItemList(WebDisplayItemList*) const override;

private:
#ifndef NDEBUG
    virtual void dumpPropertiesAsDebugString(WTF::StringBuilder&) const override final;
#endif

    const AffineTransform m_transform;
};

class PLATFORM_EXPORT EndTransformDisplayItem : public PairedEndDisplayItem {
public:
    EndTransformDisplayItem(const DisplayItemClientWrapper& client)
        : PairedEndDisplayItem(client, EndTransform) { }

    virtual void replay(GraphicsContext&) override;
    virtual void appendToWebDisplayItemList(WebDisplayItemList*) const override;

private:
#if ENABLE(ASSERT)
    virtual bool isEndAndPairedWith(DisplayItem::Type otherType) const override final { return otherType == BeginTransform; }
#endif
};

} // namespace blink

#endif // TransformDisplayItem_h
