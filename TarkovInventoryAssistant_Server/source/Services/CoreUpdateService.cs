using Interop;
using SkiaSharp;
using System.Diagnostics;
using System.Drawing.Printing;

namespace TarkovInventoryAssistant_Server.source.Services
{

    // This service will update the Singleton Core object when necessary
    // and at specific intervals to ensure price data is up to date.
    public class CoreUpdateService : BackgroundService
    {
        private readonly CoreInterop m_core;

        public CoreUpdateService(CoreInterop core) {
            m_core = core;
        }


        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {

            while (!stoppingToken.IsCancellationRequested && false)
            {
                /*
                 * Run python script to generate updated catalog. 
                 * Await Finish
                 * Run "Compile catalog"
                 * Set it as active.
                 * Delete previous catalog.
                */

                String previousCatalog = m_core.getACTIVE_CATALOG();

                String catalogName = "Catalog" + DateTime.Now.ToString("yyyy-dd-M--HH-mm-ss");
                String rawCatalogDir = m_core.getRAW_CATALOGS_DIR();

                String resultantPath = rawCatalogDir + "/" + catalogName;

                Process webScraperProcess = new Process();
                webScraperProcess.StartInfo.FileName = "C:\\MyWorkspace\\TarkovInventoryAssistant\\Scripts\\python\\python.exe";
                webScraperProcess.StartInfo.Arguments = "C:/MyWorkspace/TarkovInventoryAssistant/Scripts/TWebScraper.py" + " "
                                                + rawCatalogDir + " "
                                                + catalogName;
                webScraperProcess.Start();

                await webScraperProcess.WaitForExitAsync(stoppingToken);
                //webScraperProcess.WaitForExit();


                if (Directory.Exists(resultantPath))
                {
                    bool success = m_core.compileRawCatalog(resultantPath, catalogName, true);
                    if (success)
                    {
                        m_core.setACTIVECATALOG(m_core.getCATALOGS_DIR() + "/" + catalogName);
                        Console.WriteLine("Successfully loaded catalog: " + catalogName);
                    }
                    else
                    {
                        Console.WriteLine("CompileRawCatalog failed: " + resultantPath);
                    }

                }
                else
                {
                    Console.WriteLine("Catalog Update Failed! " + resultantPath + " does not exist.");
                }

                // Remove old Compiled Catalog.
                //try
                //{

                //    Directory.Delete(previousCatalog, true);

                //}
                //catch (Exception e)
                //{
                //    Console.WriteLine("The process failed: {0}", e.Message);
                //}


                // Wait for a certain period of time before running the next task
                await Task.Delay(TimeSpan.FromMinutes(30), stoppingToken);
            }
        }
    }
}
