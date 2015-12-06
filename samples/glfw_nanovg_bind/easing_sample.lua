glfw=require("luaglfw")
nvg=require("luananovg")
easing=require("luaeasing")
if glfw.Init() == false then
   return -1;
end

	graph={
	ANIM_TIME=1,
	DISP_TIME=2,
	progress=0,
	COUNT=200,
	background_color = nvg.RGB(55,55,55),
	frame_color = nvg.RGB(255,255,255),
	path_color = nvg.RGB(255,0,0),
    easingfunctionname = {"Liner","InSine","OutSine","InOutSine","InQuad","OutQuad","InOutQuad","InCubic","OutCubic","InOutCubic","InQuart","OutQuart","InOutQuart","InQuint","OutQuint","InOutQuint","InExpo","OutExpo","InOutExpo","InCirc","OutCirc","InOutCirc","InBack","OutBack","InOutBack","InElastic","OutElastic","InOutElastic","InBounce","OutBounce","InOutBounce"},
    current_function = "",
    
	draw_graph = function(self,ctx,x,y,w,h,t,window)
        nvg.Save(ctx);
        nvg.BeginPath(ctx);
        nvg.MoveTo(ctx,x,y);
        nvg.LineTo(ctx,x,y+h);
        nvg.LineTo(ctx,x+w,y+h);
        nvg.LineTo(ctx,x+w,y);
        nvg.LineTo(ctx,x,y);
        nvg.FillColor(ctx,self.background_color);
        nvg.Fill(ctx);
        nvg.StrokeWidth(ctx,2);
        nvg.StrokeColor(ctx,self.frame_color);
        nvg.Stroke(ctx);
        self.progress = self.progress + t
        if(self.current_function == "" or self.progress > self.ANIM_TIME + self.DISP_TIME)then        
            self.current_function = self.easingfunctionname[math.floor(math.random(1,#self.easingfunctionname))]
            print(self.current_function)
            window:setWindowTitle(self.current_function)
            self.progress = 0
            self.prev = self.next
            self.next={}
            for i = 1,self.COUNT+1 do
                self.next[i] = easing[self.current_function]((i-1),0,1,self.COUNT);
            end
            if self.prev == nil then self.prev = self.next end
        end
        local p = self.progress / self.ANIM_TIME;if p>1 then p=1 end
            
        nvg.BeginPath(ctx);
        nvg.MoveTo(ctx,x,y+h);
        for i = 1,self.COUNT+1 do
            local px = x + (w) *((i-1)/(self.COUNT));
            local py = y + h - (h) *easing[self.current_function](p,self.prev[i],self.next[i],1);
            nvg.LineTo(ctx,px,py);
        end
        nvg.StrokeWidth(ctx,2);
        nvg.StrokeColor(ctx,self.path_color);
        nvg.Stroke(ctx);   
        nvg.Restore(ctx);         
	end
	}

    -- Create a windowed mode window and its OpenGL context
    window = glfw.window.new(640, 480, "Hello World", 0, 0);
    if (window == nil) then
        glfw.Terminate();
        return -1;
    end
	N=50
    drawpath={}
    window:makeContextCurrent();
    context = nvg.Create(nvg.ANTIALIAS + nvg.STENCIL_STROKES);
    nvg.LineJoin(context,nvg.ROUND)
    nvg.LineCap(context,nvg.ROUND)
    local prevtime = os.clock();
	local black = nvg.RGB(0,0,0);
    while (window:windowShouldClose() == 0) do
        local currenttime = os.clock();
        local width,height = window:getFramebufferSize();
        nvg.glViewport(0,0,width,height);
        nvg.BeginFrame(context,width, height,1);
        
        nvg.BeginPath(context);
        nvg.Rect(context,0,0,width,height)
        
        nvg.FillColor(context,black);
        nvg.Fill(context);
		graph:draw_graph(context,width*0.1,height*0.1,width*0.8,height*0.8,currenttime-prevtime,window)
        nvg.EndFrame(context);
        
        -- Swap front and back buffers 
        window:swapBuffers();

        -- Poll for and process events 
        glfw.PollEvents();
        prevtime = currenttime;
    end

    glfw.Terminate();
    return 0;
