using Interop;
using System.Runtime.InteropServices;



CoreInterop x = new Interop.CoreInterop();
var y = x.getACTIVE_CATALOG();
x.setACTIVECATALOG("C:\\MyWorkspace\\TarkovInventoryAssistant\\Data\\CompiledCatalogs\\ItemCatalog_catalogJan16_2023_wRotations");
y = x.getACTIVE_CATALOG();


var builder = WebApplication.CreateBuilder(new WebApplicationOptions() {WebRootPath = "Source/wwwroot" });

// Add services to the container.
builder.Services.AddControllersWithViews();

var app = builder.Build();


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
