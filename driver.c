#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/of.h>

#define RTC_SECONDS 0x00
#define RTC_MINUTES 0x04
#define RTC_HOURS   0x08

static void __iomem *rtc_base;

static int rtc_probe(struct platform_device *pdev)
{
    struct resource *res;
    u32 sec, min, hr;

    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    rtc_base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(rtc_base))
        return PTR_ERR(rtc_base);

    sec = readl(rtc_base + RTC_SECONDS);
    min = readl(rtc_base + RTC_MINUTES);
    hr  = readl(rtc_base + RTC_HOURS);

    pr_info("RTC TIME -> %02u:%02u:%02u\n", hr, min, sec);

    return 0;
}

static const struct of_device_id rtc_ids[] = {
    { .compatible = "my,rtc-test" },
    { }
};
MODULE_DEVICE_TABLE(of, rtc_ids);

static struct platform_driver rtc_driver = {
    .probe = rtc_probe,
    .driver = {
        .name = "rtc_test",
        .of_match_table = rtc_ids,
    },
};

module_platform_driver(rtc_driver);

MODULE_LICENSE("GPL");
