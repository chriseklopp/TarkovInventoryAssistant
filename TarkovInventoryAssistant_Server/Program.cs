using Interop;
using Microsoft.AspNetCore.Mvc.Razor;
using System.IO;
using System.Runtime.InteropServices;
using TarkovInventoryAssistant_Server.source.Services;



var builder = WebApplication.CreateBuilder(new WebApplicationOptions() { ContentRootPath = Directory.GetCurrentDirectory()+"/source", WebRootPath = Directory.GetCurrentDirectory() + "/source/wwwroot" });
CoreInterop core = new CoreInterop();

if(args.Length > 0)
{
    if (System.IO.Directory.Exists(args[0]))
        core.setDATA_DIR(args[0]);

}


//core.setDATA_DIR("C:\\MyWorkspace\\TarkovInventoryAssistant\\TarkovInventoryAssistant_Server\\source\\wwwroot\\Data");

core.setACTIVECATALOG(core.getACTIVE_CATALOG());
builder.Services.AddSingleton<CoreInterop>(core);

builder.Services.AddHostedService<CoreUpdateService>();
// Add services to the container.
builder.Services.Configure<RazorViewEngineOptions>(options =>
{
    options.ViewLocationExpanders.Add(new CustomViewLocationExpander());
}
);
builder.Services.AddControllersWithViews();
var app = builder.Build();

app.UseRouting();
app.UseEndpoints(endpoints =>
{
    endpoints.MapControllers();
});
// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/TIA/Error");
    app.UseHsts();
}

app.UseHttpsRedirection();
app.UseStaticFiles();

app.UseRouting();

app.UseAuthorization();

app.MapControllerRoute(
    name: "default",
    pattern: "{controller=TIA}/{action=Index}/{id?}");

app.Run();
