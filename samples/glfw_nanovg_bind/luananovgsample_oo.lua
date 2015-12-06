glfw=require("luaglfw")
nvg=require("luananovg")

if glfw.Init() == false then
   return -1;
end

    -- Create a windowed mode window and its OpenGL context
    local window = glfw.window.new(640, 480, "Hello World");
    if (window == nil) then
        glfw.Terminate();
        return -1;
    end
    local drawpathx={}
    local drawpathy={}
    window:setCursorPosCallback(function(x,y) if window:getMouseButton(glfw.MOUSE_BUTTON_LEFT) == glfw.PRESS then  table.insert(drawpathx,x);table.insert(drawpathy,y) end end)
    window:setWindowCloseCallback(function(x,y) print("window closed!") end)
    
    window:makeContextCurrent();
    local context = nvg.Create(0);
    
    local red = nvg.RGB(255,0,0);
    while (window:windowShouldClose() == 0) do
        local width,height = window:getFramebufferSize();
        nvg.BeginFrame(context,width, height,1);
        nvg.StrokeColor(context,red);
        nvg.BeginPath(context);        
        for i in ipairs(drawpathx) do   
          if i==1 then
            nvg.MoveTo(context,drawpathx[i],drawpathy[i]);
          end
        nvg.LineTo(context,drawpathx[i],drawpathy[i]);
        end
        nvg.StrokeWidth(context,5);
        nvg.Stroke(context);
        nvg.EndFrame(context);
        
        -- Swap front and back buffers 
        window:swapBuffers();

        -- Poll for and process events 
        glfw.PollEvents();
    end
    glfw.Terminate();
    return 0;
