using Microsoft.AspNetCore.Builder;
using Microsoft.Extensions.FileProviders;

using Microsoft.AspNetCore.Mvc.Razor;

public class CustomViewLocationExpander : IViewLocationExpander
{
    public void PopulateValues(ViewLocationExpanderContext context)
    {
    }

    public IEnumerable<string> ExpandViewLocations(ViewLocationExpanderContext context, IEnumerable<string> viewLocations)
    {
        return new string[]
        {
            "source/Views/{1}/{0}.cshtml",
            "source/Views/Shared/{0}.cshtml"
        };
    }
}



namespace TarkovInventoryAssistant_Server
{
    public class Startup
    {

        public void Configure(IApplicationBuilder app)
        {
            var razorViewEngineOptions = app.ApplicationServices.GetRequiredService<Microsoft.AspNetCore.Mvc.Razor.RazorViewEngineOptions>();
            razorViewEngineOptions.ViewLocationExpanders.Add(new CustomViewLocationExpander());
        }
    }
}
