using Interop;
using Microsoft.AspNetCore.Mvc.Razor;
using System.Runtime.InteropServices;
using TarkovInventoryAssistant_Server.source.Services;



var builder = WebApplication.CreateBuilder(new WebApplicationOptions() { ContentRootPath = Directory.GetCurrentDirectory()+"/source" });
CoreInterop core = new CoreInterop();
// TODO: Remove hardcoding
//core.setACTIVECATALOG("C:\\MyWorkspace\\TarkovInventoryAssistant\\Data\\CompiledCatalogs\\ItemCatalog_catalogJan16_2023_wRotations");
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
