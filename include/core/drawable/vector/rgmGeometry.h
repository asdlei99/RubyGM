#pragma once
/**
* Copyright (c) 2015-2016 dustpg   mailto:dustpg@gmail.com
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

#include "rgmVector.h"
#include "../../Util/rgmUtil.h"
#include <utility>

// rubygm namespace
namespace RubyGM {
    // graphics interface for geometry
    struct IGMGeometry;
    // Drawable namespace
    namespace Drawable {
        // status for Geometry
        struct GeometryStatus : VectorStatus {
            // default value
            inline GeometryStatus() : VectorStatus() {
            }
        };
        // geometry 
        class Geometry : public Drawable::Vector {
            // super class
            using Super = Drawable::Vector;
            // dispose object
            void dispose() noexcept = 0;
        protected:
            // ctor
            Geometry(const GeometryStatus& gs) noexcept : Super(gs) {};
            // ctor
            ~Geometry() noexcept;
            // is ok?
            bool is_ok() const noexcept { return !!m_pGiGeometry; }
        public:
            // Realizations  -> to MeshEx, sf = scale factor
            auto Realization(float sf) const noexcept->Drawable::Vector*;
            // Realizations  -> to MeshEx, sf = scale factor
            auto RealizationSP(float sf) const noexcept {
                return RefPtr<Drawable::Vector>(this->Realization(sf));
            }
            // Tessellate -> to Mesh
            //auto Tessellate() const noexcept->Drawable::Mesh*;
        public:
            // render object
            void Render(IGMRenderContext& rc) const noexcept override;
        protected:
            // geometry interface
            IGMGeometry*        m_pGiGeometry = nullptr;
        };
    }
}