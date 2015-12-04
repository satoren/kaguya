SDL=require("samplesdlbind")
--sample code from https://wiki.libsdl.org/SDL_CreateRenderer
SDL.Init(SDL.INIT_VIDEO)
--can not bit and on lua5.2
local window = assert(SDL.CreateWindow("test window",SDL.WINDOWPOS_CENTERED,SDL.WINDOWPOS_CENTERED,400,400,SDL.WINDOW_RESIZABLE))
local renderer = SDL.CreateRenderer(window, -1, SDL.RENDERER_ACCELERATED);

local bitmapSurface = SDL.LoadBMP("neko-i.bmp");
local bitmapTex = SDL.CreateTextureFromSurface(renderer, bitmapSurface);
SDL.FreeSurface(bitmapSurface);
while not quit do
  event = SDL.Event.new()
  if SDL.WaitEvent(event) then
  	if SDL.QUIT == event:type() then
	  break;
	end
  end
  SDL.RenderClear(renderer);
  SDL.RenderCopy(renderer, bitmapTex, 0, 0);
  SDL.RenderPresent(renderer);
end

SDL.DestroyWindow(window)
SDL.Quit()
