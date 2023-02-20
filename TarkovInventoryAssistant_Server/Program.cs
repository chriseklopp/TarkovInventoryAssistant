using Interop;
using Microsoft.AspNetCore.Mvc.Razor;
using System.Runtime.InteropServices;
using TarkovInventoryAssistant_Server.source.Services;



//CoreInterop x = new Interop.CoreInterop();
//var y = x.getACTIVE_CATALOG();
//x.setACTIVECATALOG("C:\\MyWorkspace\\TarkovInventoryAssistant\\Data\\CompiledCatalogs\\ItemCatalog_catalogJan16_2023_wRotations");
//y = x.getACTIVE_CATALOG();
//x.detectImageContent(69);

var builder = WebApplication.CreateBuilder(new WebApplicationOptions() { ContentRootPath = Directory.GetCurrentDirectory()+"/source" });
CoreInterop core = new CoreInterop();
core.setACTIVECATALOG("C:\\MyWorkspace\\TarkovInventoryAssistant\\Data\\CompiledCatalogs\\ItemCatalog_catalogJan16_2023_wRotations");
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
    app.UseExceptionHandler("/Home/Error");
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
}

app.UseHttpsRedirection();
app.UseStaticFiles();

app.UseRouting();

app.UseAuthorization();

app.MapControllerRoute(
    name: "default",
    pattern: "{controller=Home}/{action=Index}/{id?}");

app.Run();
