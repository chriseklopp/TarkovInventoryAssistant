using Interop;
using Microsoft.AspNetCore.Mvc;
using SkiaSharp;
using System.Net.Mime;
using System.Text;
using static SkiaSharp.SKImageFilter;
using static System.Net.Mime.MediaTypeNames;

namespace TarkovInventoryAssistant_Server.Models
{
    public class DetectionResultsModel
    {

        // Convert a single DRMarshall to a a DetectionResultsModel.
        public DetectionResultsModel(DetectionResultMarshal drm, string catalogPath, int itemCount = 1)
        {
            string filePath = catalogPath + "/images/" + drm.name + ".webp";

            catalogImage = filePath;
            name = drm.name;
            dim = drm.dimHeight.ToString() + 'x' + drm.dimWidth.ToString();

            int nullIndex = Array.IndexOf(drm.fleaUnit, (byte)0);
            string fleaUnit = Encoding.UTF8.GetString(drm.fleaUnit, 0, nullIndex);
            fleaPrice = fleaUnit + (drm.fleaprice * itemCount).ToString("N0");
            pricePerSlot = fleaUnit + (drm.fleaPricePerSlot * itemCount).ToString("N0");

            nullIndex = Array.IndexOf(drm.traderUnit, (byte)0);
            string traderunit = Encoding.UTF8.GetString(drm.traderUnit, 0, nullIndex);
            traderPrice = traderunit + (drm.traderPrice * itemCount).ToString("N0");
            trader = drm.trader;

            sellOnFlea = drm.sellOnFlea;

            count = itemCount;
        }


        public string? RequestId { get; set; }

        public string catalogImage { get; set; }

        public string name { get; set; } = "";

        public string dim { get; set; } = "";

        public string fleaPrice { get; set; }

        public string pricePerSlot { get; set; }

        public string traderPrice { get; set; }

        public string trader { get; set; } = "";

        public bool sellOnFlea { get; set; }

        public int count { get; set; }
    }
}