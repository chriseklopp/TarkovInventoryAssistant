using Interop;

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
            while (!stoppingToken.IsCancellationRequested)
            {
                /*
                 * Run python script to generate updated catalog. 
                 * Await Finish
                 * Run "Compile catalog"
                 * Set it as active.
                 * Delete previous catalog.
                */

                //m_core.compileRawCatalog(m_core.getRAW_CATALOGS_DIR(), "coolNameWithTime", true);
                //m_core.setACTIVECATALOG();


                // Wait for a certain period of time before running the next task
                await Task.Delay(TimeSpan.FromMinutes(30), stoppingToken);
            }
        }
    }
}
