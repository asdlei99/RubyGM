
- RubyGM                                命名空间
  - CGMSprite                           精灵类-用于显示Drawble对象
  - Resource                            资源对象命名空间
    - Base                              基类
  - Drawable                            可渲染对象命名空间
    - Object        < Resource::Base    基类
    - Vector        < Object            矢量对象
    - ===============基本矢量对象================
    - Line          < Vector            线段几何
    - Rect          < Vector            矩形几何
    - Ellipse       < Vector            椭圆几何
    - Path          < Vector            路径几何
    - Polyline      < Vector            折线几何
    - Glyph         < Vector            字形几何
    - Geometry      < Vector            几何对象
    - ===============复杂矢量对象================
    - Textlayout    < Object            文本布局
    - ===============基本位图对象================
    - Bitmap        < Object            位图
  - Asset                               Asset资源对象命名空间
    - Object        < Resource::Base    基类
    - Font          < Object            文本格式/字体资源
    - Brush         < Object            笔刷资源
      - ColorBrush  < Brush             纯色笔刷资源
      - XXXXXXBrush < Brush             XXX笔刷资源
      - BitmapBrush < Brush             位图笔刷资源
    - Bitmap        < Object            位图资源 
      - FileBitmap  < Bitmap            文件位图资源, 来自图片文件
      - StreamBitmap< Bitmap            流式位图资源, 来自数据流
      - RasterBitmap< Bitmap            光栅化位图资源, 可渲染对象光栅化结果
      
      - RasterEx    < Bitmap            高级光栅化位图资源,会储存结果到硬盘上,消耗可观时
                                        间(异步IO去掉IO时间花费), 可能重建失败. 若失败会
                                        以粉紫色/黑色间隔色块显示.用于多级精灵光栅化.
                                        
      - BufferBitmap< Bitmap            使用缓存创建的位图,如果位图过大,会储存结果到硬盘上, 
                                        消耗可观时间(异步IO去掉IO时间花费), 可能重建失败.
                                        若失败会以粉紫色/黑色间隔色块显示.

----

## 复杂引用例子
  



## Sprite

##### 属性: <font color="red">x</font>
浮点 相对于父精灵的x坐标

##### 属性: <font color="red">y</font>
浮点 相对于父精灵的y坐标

##### 属性: <font color="red">z</font>
整型 同一级精灵中渲染优先级，越高表示越迟渲染，以显示在"高"的地方
